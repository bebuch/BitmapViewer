//-----------------------------------------------------------------------------
// Copyright (c) 2013-2018 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <BitmapViewer/file_handler.hpp>

#include <bitmap/binary_read.hpp>

#include <big.hpp>

#include <QImage>


namespace bitmap_viewer{


	namespace{


		template < bool is_floating_point >
		struct init{
			template < typename T >
			static std::size_t undefined_count(bitmap< T > const& bitmap);

			template < typename T >
			static std::tuple< T, T > minmax(bitmap< T > const& bitmap);
		};

		template <>
		template < typename T >
		std::size_t init< true >::undefined_count(bitmap< T > const& bitmap){
			return std::count_if(bitmap.cbegin(), bitmap.cend(),
				[](T v){return v != v;});
		}

		template <>
		template < typename T >
		std::size_t init< false >::undefined_count(bitmap< T > const&){
			return 0;
		}

		template <>
		template < typename T >
		std::tuple< T, T > init< true >::minmax(bitmap< T > const& bitmap){
			auto iter = bitmap.cbegin();
			while(iter != bitmap.cend() && std::isnan(*iter)) ++iter;
			if(iter == bitmap.cend()) return std::make_tuple(T(), T());

			auto result = std::make_tuple(*iter, *iter);
			std::for_each(++iter, bitmap.end(), [&result](T const& value){
				if(value < std::get< 0 >(result)){
					std::get< 0 >(result) = value;
				}else if(value > std::get< 1 >(result)){
					std::get< 1 >(result) = value;
				}
			});
			return result;
		}

		template <>
		template < typename T >
		std::tuple< T, T > init< false >::minmax(bitmap< T > const& bitmap){
			if(!bitmap.point_count()) return std::make_tuple(T(), T());

			auto result = std::minmax_element(bitmap.cbegin(), bitmap.cend());
			return std::make_tuple(
				*std::get< 0 >(result), *std::get< 1 >(result));
		}

		template < typename T >
		bitmap_viewer::bitmap< T >
		load_big(std::ifstream& is, big::header header){
			bitmap_viewer::bitmap< T > bitmap(
				size< std::size_t >(header.width, header.height)
			);
			big::read_data(bitmap, is);
			return bitmap;
		}


	}


	template < typename T >
	bitmap_info< T >::bitmap_info(bitmap_viewer::bitmap< T >&& image):
		bitmap(std::move(image)),
		minmax(init< std::is_floating_point< T >::value >::minmax(bitmap)),
		undefined_count(init< std::is_floating_point< T >::value >
			::undefined_count(bitmap))
		{}


	bitmap_type load_big(std::string const& filename){
		std::ifstream is(
			filename.c_str(),
			std::ios_base::in | std::ios_base::binary
		);

		if(!is.is_open()){
			throw std::runtime_error("Can not open file: " + filename);
		}

		big::header header = big::read_header(is);

		switch(header.type){
			case big::type< std::uint8_t >::value:
				return bitmap_info< std::uint8_t >(
					load_big< std::uint8_t >(is, header));
			case big::type< std::uint16_t >::value:
				return bitmap_info< std::uint16_t >(
					load_big< std::uint16_t >(is, header));
			case big::type< std::uint32_t >::value:
				return bitmap_info< std::uint32_t >(
					load_big< std::uint32_t >(is, header));
			case big::type< std::uint64_t >::value:
				return bitmap_info< std::uint64_t >(
					load_big< std::uint64_t >(is, header));
			case big::type< float >::value:
				return bitmap_info< float >(
					load_big< float >(is, header));
			case big::type< double >::value:
				return bitmap_info< double >(
					load_big< double >(is, header));
			case big::type< long double >::value:
				return bitmap_info< long double >(
					load_big< long double >(is, header));
		}

		throw std::runtime_error(filename + " has unknown file type: " +
			std::to_string(header.type));
	}

	bitmap_type load_bbf(std::string const& filename){
		std::ifstream is(
			filename.c_str(),
			std::ios_base::in | std::ios_base::binary
		);

		if(!is.is_open()){
			throw std::runtime_error("Can not open file: " + filename);
		}

		auto header = bmp::binary_read_header(is);

		if(header.channel_count != 1){
			std::ostringstream os;
			os << filename << " has "
				<< static_cast< int >(header.channel_count)
				<< " channels, but images with more than one channels are "
					"not supported";
			throw std::runtime_error(os.str());
		}

		using bmp::detail::binary_type_flags;
		auto const type_flag = binary_type_flags(header.flags & 0x0F);
		switch(type_flag){
		case binary_type_flags::is_unsigned:
			switch(header.channel_size){
			case 1: return bitmap_info(bmp::binary_read_data< std::uint8_t >(
					is, header, false));
			case 2: return bitmap_info(bmp::binary_read_data< std::uint16_t >(
					is, header, false));
			case 4: return bitmap_info(bmp::binary_read_data< std::uint32_t >(
					is, header, false));
			case 8: return bitmap_info(bmp::binary_read_data< std::uint64_t >(
					is, header, false));
			}
		break;
		case binary_type_flags::is_signed:
			switch(header.channel_size){
			case 1: return bitmap_info(bmp::binary_read_data< std::int8_t >(
					is, header, false));
			case 2: return bitmap_info(bmp::binary_read_data< std::int16_t >(
					is, header, false));
			case 4: return bitmap_info(bmp::binary_read_data< std::int32_t >(
					is, header, false));
			case 8: return bitmap_info(bmp::binary_read_data< std::int64_t >(
					is, header, false));
			}
		break;
		case binary_type_flags::is_floating_point:
			switch(header.channel_size){
			case 4: return bitmap_info(bmp::binary_read_data< float >(
					is, header, false));
			case 8: return bitmap_info(bmp::binary_read_data< double >(
					is, header, false));
			}
		break;
		case binary_type_flags::is_bool:
			return bmp::binary_read_data< bool >(is, header, false);
		break;
		}

		throw std::runtime_error(filename + ": unsupported value size");
	}

	bitmap_type load_png(QString const& filename){
		QImage image;
		if(!image.load(filename, "PNG")){
			throw std::runtime_error("Can not load '" +
				filename.toStdString() + "' as PNG-File");
		}
		if(!image.isGrayscale()){
			throw std::logic_error("'" + filename.toStdString() +
				"' is not a grayscale image");
		}
		image = image.convertToFormat(QImage::Format_Grayscale8);
		bitmap< std::uint8_t > result(
			size< std::size_t >(image.width(), image.height())
		);
		for(int y = 0; y < image.height(); ++y){
			auto ptr = image.constScanLine(y);
			std::copy(ptr, ptr + image.width(),
				result.data() + image.width() * y);
		}
		return bitmap_info< std::uint8_t >(std::move(result));
	}


}
