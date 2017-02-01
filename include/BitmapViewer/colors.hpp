//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _bitmap_viewer__colors__hpp_INCLUDED_
#define _bitmap_viewer__colors__hpp_INCLUDED_

#include <QColor>
#include <QBrush>

#include <boost/signals2.hpp>


namespace bitmap_viewer{


	class colors{
	public:
		enum class type { rainbow, gray };

		colors(type t = type::rainbow, unsigned strips = 256);

		boost::signals2::signal< void() > update;

		std::uint16_t fold()const;
		unsigned strips()const;
		void set_fold(std::uint16_t fold);
		void set_strips(unsigned strips);

		unsigned step_pos(unsigned pos)const;
		unsigned pass_step_pos(unsigned step_pos)const;
		unsigned pass_pos(unsigned pos)const;

		void contrast_line(bool enable);
		void next_palette();

		QBrush const& background_brush()const;
		QColor operator()(unsigned pos)const;

	private:
		bool contrast_line_ = false;
		std::uint16_t fold_;
		unsigned strips_;
		type type_;
	};


}


#endif
