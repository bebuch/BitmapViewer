//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _bitmap_viewer__slider__hpp_INCLUDED_
#define _bitmap_viewer__slider__hpp_INCLUDED_

#include <QWidget>

#include "colors.hpp"


namespace bitmap_viewer{


	class slider: public QWidget{
		Q_OBJECT

	public:
		slider(QWidget* parent);

		bitmap_viewer::colors colors;

		unsigned shift()const{return shift_;}

	public slots:
		void set_shift(unsigned pos);
		void set_strips(unsigned count);

		void contrast_line(bool enable);

		void next_palette();

		void right_shift();
		void left_shift();

		void fold_linear();
		void unfold_linear();
		void fold_exponential();
		void unfold_exponential();

	signals:
		void shift_changed(unsigned);
		void change();

	protected:
		virtual void mouseMoveEvent(QMouseEvent* event);
		virtual void mousePressEvent(QMouseEvent* event);
		virtual void mouseReleaseEvent(QMouseEvent* event);
		virtual void paintEvent(QPaintEvent * event);
		virtual void wheelEvent(QWheelEvent* event);

	private:
		bool active_;
		unsigned shift_;
		unsigned startpos_;
		unsigned startshift_;
	};


}


#endif
