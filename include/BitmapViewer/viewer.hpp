//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
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

		void set_bitmap(item const*);
		void set_slider(slider const*);

		void draw_item(
			QPainter& painter,
			item const* item,
			QRectF const& rect
		)const;

	public slots:
		void next_mode();

	signals:
		void info(QString);

	protected:
		virtual void mouseMoveEvent(QMouseEvent* event);
		virtual void paintEvent(QPaintEvent* event);

	private:
		item const* item_;
		slider const* slider_;
		mode mode_;
	};


}


#endif
