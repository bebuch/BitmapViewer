//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _bitmap_viewer__file_handler__hpp_INCLUDED_
#define _bitmap_viewer__file_handler__hpp_INCLUDED_

#include "tools/matrix.hpp"
#include "tools/big.hpp"

#include <boost/variant.hpp>

#include <algorithm>


namespace bitmap_viewer{


	template < bool is_floating_point >
	struct init{
		template < typename T >
		static std::size_t undefined_count(tools::bitmap< T > const& bitmap);

		template < typename T >
		static std::tuple< T, T > minmax(tools::bitmap< T > const& bitmap);
	};

	template <>
	template < typename T >
	std::size_t init< true >::undefined_count(tools::bitmap< T > const& bitmap){
		return std::count_if(bitmap.cbegin(), bitmap.cend(),
			[](T v){return v != v;});
	}

	template <>
	template < typename T >
	std::size_t init< false >::undefined_count(tools::bitmap< T > const&){
		return 0;
	}

	template <>
	template < typename T >
	std::tuple< T, T > init< true >::minmax(tools::bitmap< T > const& bitmap){
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
	std::tuple< T, T > init< false >::minmax(tools::bitmap< T > const& bitmap){
		if(!point_count(bitmap)) return std::make_tuple(T(), T());

		auto result = std::minmax_element(bitmap.cbegin(), bitmap.cend());
		return std::make_tuple(*std::get< 0 >(result), *std::get< 1 >(result));
	}

	template < typename T >
	class bitmap_info{
	public:
		typedef T value_type;

		bitmap_info(){}

		bitmap_info(std::ifstream& is, tools::big::header header):
			bitmap(load(is, header)),
			minmax(init< std::is_floating_point< T >::value >::minmax(bitmap)),
			undefined_count(init< std::is_floating_point< T >::value >
				::undefined_count(bitmap))
			{}

		tools::bitmap< T > bitmap;
		std::tuple< T, T > minmax;
		std::size_t undefined_count;

	private:
		static tools::bitmap< T > load(
			std::ifstream& is, tools::big::header header
		){
			tools::bitmap< T > bitmap(
				tools::dimensions< 0, 0 >(header.cols, header.rows)
			);
			tools::big::read_data(bitmap, is);
			return bitmap;
		}
	};


	typedef boost::variant<
			bitmap_info< bool >,
			bitmap_info< std::uint8_t >,
			bitmap_info< std::uint16_t >,
			bitmap_info< std::uint32_t >,
			bitmap_info< std::uint64_t >,
			bitmap_info< float >,
			bitmap_info< double >,
			bitmap_info< long double >
		> bitmap_type;

	bitmap_type load(std::string const& filename);


}


#endif
