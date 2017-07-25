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

#include <bitmap/bitmap.hpp>

#include <boost/variant.hpp>

#include <QString>

#include <algorithm>


namespace bitmap_viewer{


	using ::bmp::bitmap;
	using ::bmp::point;
	using ::bmp::size;


	template < typename T >
	class bitmap_info{
	public:
		using value_type = T;

		bitmap_info(){}
		bitmap_info(bitmap_viewer::bitmap< T >&& image);

		bitmap_viewer::bitmap< T > bitmap;
		std::tuple< T, T > minmax;
		std::size_t undefined_count;
	};


	typedef boost::variant<
			bitmap_info< bool >,
			bitmap_info< std::int8_t >,
			bitmap_info< std::int16_t >,
			bitmap_info< std::int32_t >,
			bitmap_info< std::int64_t >,
			bitmap_info< std::uint8_t >,
			bitmap_info< std::uint16_t >,
			bitmap_info< std::uint32_t >,
			bitmap_info< std::uint64_t >,
			bitmap_info< float >,
			bitmap_info< double >,
			bitmap_info< long double >
		> bitmap_type;

	bitmap_type load_big(std::string const& filename);
	bitmap_type load_bbf(std::string const& filename);
	bitmap_type load_png(QString const& filename);


}


#endif
