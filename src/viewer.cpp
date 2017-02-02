//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <BitmapViewer/viewer.hpp>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

#include <sstream>


namespace bitmap_viewer{


	namespace{

		template < typename T >
		QString toQString(T const& v){
			std::ostringstream os;
			os << v;
			return os.str().c_str();
		}

		class print_value: public boost::static_visitor< QString >{
		public:
			print_value(bitmap_viewer::point< std::size_t > const& point):
				point(point){}

			template < typename T >
			QString operator()(T const& info)const{
				if(!is_point_in_bitmap(info.bitmap, point)) return QString();
				auto const value = static_cast< double >(info.bitmap(point));
				return QString(QObject::tr("Position: %1, %2; Value: %3"))
					.arg(point.x())
					.arg(point.y())
					.arg(toQString(value));
			}

		private:
			bitmap_viewer::point< std::size_t > const point;
		};

	}


	viewer::viewer():
		item_(nullptr),
		slider_(nullptr),
		mode_(mode::pass)
	{
		setMouseTracking(true);
	}

	void viewer::set_bitmap(item const* i){
		item_ = i;
		if(mode_ == mode::scroll && item_){
			setMinimumSize(item_->width(), item_->height());
		}
		auto_range_set();
		repaint();

		if(!slider_) return;
		slider_settings_changed(
			slider_->min(),
			slider_->max(),
			slider_->auto_range(),
			slider_->int_range()
		);
	}

	void viewer::set_slider(slider* s){
		slider_ = s;
		repaint();
	}

	void viewer::next_mode(){
		switch(mode_){
		case mode::scroll:
			mode_ = mode::scale;
			setMinimumSize(0, 0);
		break;
		case mode::scale:
			mode_ = mode::pass;
			setMinimumSize(0, 0);
		break;
		case mode::pass:
			mode_ = mode::scroll;
			if(item_) setMinimumSize(item_->width(), item_->height());
		break;
		}
		repaint();
	}

	void viewer::auto_range_set(){
		if(!slider_ || !item_) return;

		if(slider_->auto_range()){
			double min, max;
			if(slider_->int_range() && !item_->is_float()){
				std::tie(min, max) = item_->minmax_type_values();
			}else{
				std::tie(min, max) = item_->minmax_values();
			}
			slider_->set_min(min);
			slider_->set_max(max);
		}
	}

	void viewer::set_slider_settings(
		double min,
		double max,
		bool auto_range,
		bool int_range,
		colors::contrast_line_type c
	){
		if(!slider_) return;

		slider_->set_contrast_type(c);

		if(!item_) return;

		slider_->set_auto_range(auto_range);
		slider_->set_int_range(int_range);

		if(!auto_range){
			slider_->set_min(min);
			slider_->set_max(max);
		}else{
			auto_range_set();
		}

		repaint();

		slider_settings_changed(
			slider_->min(),
			slider_->max(),
			slider_->auto_range(),
			slider_->int_range()
		);
	}

	void viewer::mouseMoveEvent(QMouseEvent* event){
		if(!slider_ || !item_) return;

		auto point = event->localPos();

		double iw = item_->width();
		double ih = item_->height();
		double factor = std::max(iw / width(), ih / height());
		if(mode_ != mode::scale && factor < 1) factor = 1;
		point *= factor;

		auto p = bitmap_viewer::point< std::size_t >(
			static_cast< std::size_t >(point.x()),
			static_cast< std::size_t >(point.y())
		);
		info(boost::apply_visitor(print_value(p), item_->bitmap()));
	}

	void viewer::paintEvent(QPaintEvent*){
		if(!slider_ || !item_) return;

		double iw = item_->width();
		double ih = item_->height();
		double factor = std::min(width() / iw, height() / ih);
		if(mode_ != mode::scale && factor > 1) factor = 1;

		QPainter painter(this);
		QRectF rect(0, 0, iw * factor, ih * factor);

		draw_item(painter, item_, rect);
	}

	void viewer::draw_item(
		QPainter& painter,
		item const* item,
		QRectF const& rect
	)const{
		painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
		painter.setPen(Qt::NoPen);
		painter.setBrush(slider_->colors().background_brush());
		painter.drawRect(rect);
		painter.drawImage(rect, item->image(
			slider_->colors(), slider_->shift()));
	}


}
