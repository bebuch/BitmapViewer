//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <BitmapViewer/item.hpp>

#include <QPainter>

#include <type_traits>
#include <sstream>


namespace bitmap_viewer{

	namespace{

		constexpr double unsigned_max =
			std::numeric_limits< unsigned >::max();

		constexpr double unsigned_value_count =
			unsigned_max + 1.0;

	}


	class generate_image: public boost::static_visitor< QImage >{
	public:
		generate_image(
			colors const& color, unsigned shift
		): color_(color), shift_(shift){}

		template < typename T >
		QImage operator()(T const& info)const{
			typedef typename T::value_type value_type;

			auto const& bitmap = info.bitmap;
			QImage image(
				bitmap.width(), bitmap.height(), QImage::Format_ARGB32);

			auto const min = std::get< 0 >(info.minmax);
			auto const max = std::get< 1 >(info.minmax);
			auto const diff = static_cast< double >(max - min);

			std::transform(bitmap.begin(), bitmap.end(),
				reinterpret_cast< unsigned* >(image.bits()),
				[this, min, diff](value_type value)->unsigned{
					if(
						std::is_floating_point< typename T::value_type >::value
						&& std::isnan(value)
					){
						return QColor(0, 0, 0, 0).rgba();
					}else{
						auto const vdiff = (value - min) / diff;
						auto const shift_value =
							static_cast< unsigned >(vdiff * unsigned_max);
						return color_(shift_value + shift_).rgba();
					}
				}
			);

			return image;
		}

	private:
		colors const& color_;
		unsigned shift_;
	};


//	template <>
//	QImage generate_image::operator()(bitmap_info< bool > const& info)const{
//		auto const& bitmap = info.bitmap;
//		QImage image(bitmap.width(), bitmap.height(), QImage::Format_ARGB32);

//		std::transform(bitmap.begin(), bitmap.end(),
//			reinterpret_cast< unsigned* >(image.bits()),
//			[this](bool value)->unsigned{
//				return color_((value ? (unsigned() - 1) / 2 : 0) + shift_)
//					.rgba();
//			}
//		);

//		return image;
//	}

	class generate_icon: public boost::static_visitor< QPixmap >{
	private:
		template < typename T >
		struct palette{
			static constexpr auto value =
				std::is_floating_point< T >::value
				? colors::type::rainbow
				: colors::type::gray;
		};

		template < typename T >
		static constexpr auto palette_v = palette< T >::value;

	public:
		template < typename T >
		QPixmap operator()(T const& info)const{
			constexpr auto palette = palette_v< typename T::value_type >;

			auto const color = colors(palette);

			auto image = generate_image(color, 0)(info)
				.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);

			QPixmap pixmap(image.width(), image.height());

			{
				QPainter painter(&pixmap);
				QRectF rect(0, 0, pixmap.width(), pixmap.height());
				painter.setPen(Qt::NoPen);
				painter.setBrush(color.background_brush());
				painter.drawRect(rect);
				painter.drawImage(rect, image);
			}

			return pixmap;
		}
	};


	template < typename T >
	QString toQString(T const& v){
		std::ostringstream os;
		os << v;
		return os.str().c_str();
	}

	template <>
	QString toQString(std::uint8_t const& v){
		std::ostringstream os;
		os << static_cast< unsigned >(v);
		return os.str().c_str();
	}

	template <>
	QString toQString(bool const& v){
		return v ? QString("true") : QString("false");
	}

	struct print_min: public boost::static_visitor< QString >{
		template < typename T >
		QString operator()(T const& info)const{
			return toQString(std::get< 0 >(info.minmax));
		}
	};

	struct print_max: public boost::static_visitor< QString >{
		template < typename T >
		QString operator()(T const& info)const{
			return toQString(std::get< 1 >(info.minmax));
		}
	};

	struct print_type: public boost::static_visitor< QString >{
		template < typename T >
		QString operator()(T const&)const{
			if(std::is_floating_point< typename T::value_type >::value){
				return QString(QObject::tr("Floating Point (%1 Byte)"))
					.arg(sizeof(typename T::value_type));
			}else if(
				std::is_same<
					typename std::remove_cv< typename T::value_type >::type,
					bool
				>::value
			){
				return QString(QObject::tr("Boolean"));
			}else{
				return QString(QObject::tr("Integer (%1 Byte)"))
					.arg(sizeof(typename T::value_type));
			}
		}
	};

	struct print_undefined_count: public boost::static_visitor< std::size_t >{
		template < typename T >
		std::size_t operator()(T const& info)const{
			return info.undefined_count;
		}
	};

	struct print_width: public boost::static_visitor< std::size_t >{
		template < typename T >
		std::size_t operator()(T const& info)const{
			return info.bitmap.width();
		}
	};

	struct print_height: public boost::static_visitor< std::size_t >{
		template < typename T >
		std::size_t operator()(T const& info)const{
			return info.bitmap.height();
		}
	};


	item::item(){}

	item::item(QString const& filename):
		bitmap_(load(filename.toStdString())),
		filename_(filename),
		icon_(boost::apply_visitor(generate_icon(), bitmap_))
		{}

	QString item::filename()const{
		return filename_.fileName();
	}

	QString item::path()const{
		return filename_.absolutePath();
	}

	QString item::path_and_file()const{
		return filename_.absoluteFilePath();
	}

	QString item::min()const{
		return boost::apply_visitor(print_min(), bitmap_);
	}

	QString item::max()const{
		return boost::apply_visitor(print_max(), bitmap_);
	}

	QString item::type()const{
		return boost::apply_visitor(print_type(), bitmap_);
	}

	std::size_t item::undefined_count()const{
		return boost::apply_visitor(print_undefined_count(), bitmap_);
	}

	std::size_t item::width()const{
		return boost::apply_visitor(print_width(), bitmap_);
	}

	std::size_t item::height()const{
		return boost::apply_visitor(print_height(), bitmap_);
	}

	QImage item::image(colors const& color, unsigned shift)const{
		return boost::apply_visitor(generate_image(color, shift), bitmap_);
	}

	QPixmap item::icon()const{
		return icon_;
	}


}
