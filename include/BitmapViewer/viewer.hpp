//-----------------------------------------------------------------------------
// Copyright (c) 2013-2018 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _bitmap_viewer__viewer__hpp_INCLUDED_
#define _bitmap_viewer__viewer__hpp_INCLUDED_

#include "item.hpp"
#include "slider.hpp"

#include <QWidget>


namespace bitmap_viewer{


	class viewer: public QWidget{
		Q_OBJECT

	public:
		enum class mode { scroll, scale, pass };

		viewer();

		void set_bitmap(item*);
		void set_slider(slider*);

		void draw_item(
			QPainter& painter,
			item const* item,
			QRectF const& rect
		)const;

	public slots:
		void next_mode();
		void set_slider_settings(
			double min,
			double max,
			bool auto_range,
			bool int_range,
			colors::contrast_line_type c
		);

	signals:
		void info(QString);
		void slider_settings_changed(
			double min,
			double max,
			bool auto_range,
			bool int_range
		);
		void point_list_changed(QStringList);

	protected:
		virtual void mouseMoveEvent(QMouseEvent* event);
		virtual void mouseReleaseEvent(QMouseEvent* event);
		virtual void paintEvent(QPaintEvent* event);

	private:
		void auto_range_set();
		void update_point_list();

		double local_item_factor()const;
		point< std::size_t > to_item_point(QPointF p)const;
		QPointF to_local_point(point< std::size_t > p)const;

		item* item_;
		slider* slider_;
		mode mode_;
	};


}


#endif
