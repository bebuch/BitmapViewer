//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <BitmapViewer/file_handler.hpp>


namespace bitmap_viewer{


	bitmap_type load(std::string const& filename){
		std::ifstream is(
			filename.c_str(),
			std::ios_base::in | std::ios_base::binary
		);

		if(!is.is_open()){
			throw std::runtime_error("Can not open file: " + filename);
		}

		tools::big::header header = tools::big::read_header(is);

		using tools::big::type;
		switch(header.type){
			case type< bool >::value():
				return bitmap_info< bool >(is, header);
			case type< std::uint8_t >::value():
				return bitmap_info< std::uint8_t >(is, header);
			case type< std::uint16_t >::value():
				return bitmap_info< std::uint16_t >(is, header);
			case type< std::uint32_t >::value():
				return bitmap_info< std::uint32_t >(is, header);
			case type< std::uint64_t >::value():
				return bitmap_info< std::uint64_t >(is, header);
			case type< float >::value():
				return bitmap_info< float >(is, header);
			case type< double >::value():
				return bitmap_info< double >(is, header);
			case type< long double >::value():
				return bitmap_info< long double >(is, header);
		}

		throw std::runtime_error(filename + " has unknown file type: " +
			std::to_string(header.type));
	}


}
