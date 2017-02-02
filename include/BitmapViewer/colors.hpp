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
		enum class palette_type { rainbow, gray };
		enum class contrast_line_type: int{
			auto_color,
			auto_palette
		};

		colors(palette_type t = palette_type::rainbow, unsigned strips = 256);

		boost::signals2::signal< void() > update;

		std::uint16_t fold()const;
		unsigned strips()const;
		double min()const;
		double max()const;
		bool auto_range()const;
		bool int_range()const;

		void set_fold(std::uint16_t fold);
		void set_strips(unsigned strips);
		void set_min(double pos);
		void set_max(double pos);
		void set_auto_range(bool on);
		void set_int_range(bool on);

		unsigned step_pos(unsigned pos)const;
		unsigned pass_step_pos(unsigned step_pos)const;
		unsigned pass_pos(unsigned pos)const;

		void contrast_line(bool enable);
		void next_palette();

		QBrush const& background_brush()const;
		QColor operator()(unsigned pos)const;

		void set_contrast_type(contrast_line_type c);

		static std::vector< std::tuple< QString, contrast_line_type > >
			contrast_lines();

	private:
		bool contrast_line_ = false;
		std::uint16_t fold_;
		unsigned strips_;
		double min_;
		double max_;
		bool auto_range_;
		bool int_range_;
		palette_type type_;
		contrast_line_type contrast_type_;
	};


}

Q_DECLARE_METATYPE(bitmap_viewer::colors::contrast_line_type)


#endif
