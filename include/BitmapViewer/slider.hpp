//-----------------------------------------------------------------------------
// Copyright (c) 2013-2018 Benjamin Buch
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

		unsigned shift()const{return shift_;}

		double min()const{ return colors_.min(); }
		double max()const{ return colors_.max(); }
		bool auto_range()const{ return colors_.auto_range(); }
		bool int_range()const{ return colors_.int_range(); }

		void set_min(double pos){ return colors_.set_min(pos); }
		void set_max(double pos){ return colors_.set_max(pos); }
		void set_auto_range(bool on){ return colors_.set_auto_range(on); }
		void set_int_range(bool on){ return colors_.set_int_range(on); }

		void set_contrast_type(colors::contrast_line_type c){
			colors_.set_contrast_type(c);
			repaint();
		}


		bitmap_viewer::colors const& colors()const{ return colors_; }

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
		unsigned inc_step(unsigned step, int inc)const;

		bitmap_viewer::colors colors_;

		bool active_;
		unsigned shift_;
		unsigned startpos_;
		unsigned startshift_;
	};


}


#endif
