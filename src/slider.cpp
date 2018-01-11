//-----------------------------------------------------------------------------
// Copyright (c) 2013-2018 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <BitmapViewer/slider.hpp>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QPainter>

#include <cmath>


namespace bitmap_viewer{


	constexpr double shift_range = std::numeric_limits< unsigned >::max() + 1.;


	slider::slider(QWidget* parent):
		QWidget(parent),
		active_(false),
		shift_(0),
		startpos_(0),
		startshift_(0)
	{
		setCursor(Qt::OpenHandCursor);
		colors_.update.connect([this]{repaint();change();});
	}

	void slider::set_shift(unsigned s){
		shift_ = colors_.pass_pos(s);
		repaint();
		shift_changed(s);
		change();
	}

	void slider::set_strips(unsigned count){
		colors_.set_strips(count);
		set_shift(shift());
	}

	void slider::contrast_line(bool enable){
		colors_.contrast_line(enable);
	}

	void slider::next_palette(){
		colors_.next_palette();
	}

	unsigned slider::inc_step(unsigned step, int inc)const{
		if(inc < 0){
			inc = -inc % colors_.strips();
			step += colors_.strips() - inc;
		}else{
			step += inc;
			step %= colors_.strips();
		}
		return step;
	}

	void slider::right_shift(){
		auto step_pos = colors_.step_pos(shift_);
		step_pos = inc_step(step_pos, -1);
		set_shift(colors_.pass_step_pos(step_pos));
	}

	void slider::left_shift(){
		auto step_pos = colors_.step_pos(shift_);
		step_pos = inc_step(step_pos, 1);
		set_shift(colors_.pass_step_pos(step_pos));
	}

	void slider::fold_linear(){
		colors_.set_fold(colors_.fold() + 1);
	}

	void slider::unfold_linear(){
		colors_.set_fold(colors_.fold() - 1);
	}

	void slider::fold_exponential(){
		colors_.set_fold(colors_.fold() << 1);
	}

	void slider::unfold_exponential(){
		colors_.set_fold(colors_.fold() >> 1);
	}

	void slider::mouseMoveEvent(QMouseEvent* event){
		if(!active_) return;

		unsigned w = width();
		set_shift(startshift_ + startpos_ - event->x() * (shift_range / w));
	}

	void slider::mousePressEvent(QMouseEvent* event){
		if(event->button() != Qt::LeftButton) return;

		active_ = true;
		setCursor(Qt::ClosedHandCursor);

		unsigned w = width();
		startpos_ = event->x() * (shift_range / w);
		startshift_ = shift_;
	}

	void slider::mouseReleaseEvent(QMouseEvent* event){
		if(event->button() != Qt::LeftButton) return;

		setCursor(Qt::OpenHandCursor);
		active_ = false;
	}

	void slider::paintEvent(QPaintEvent*){
		QPainter painter(this);
		unsigned w = width();
		for(unsigned i = 0; i < w; ++i){
			painter.setPen(colors_(shift_ + i * (shift_range / w)));
			painter.drawLine(i, 0, i, height());
		}

		auto text = QString("Pos: %1").arg(colors_.step_pos(shift_));
		auto flags = Qt::AlignLeft | Qt::AlignVCenter;
		auto fontfactor = 0.7;
		auto fontsize = height() * fontfactor;
		auto x_offset = height() * (1 - fontfactor) / 2;
		auto p1 = QPointF(x_offset, 0);
		auto p2 = QPointF(w - 2*x_offset, height());

		QFont font;
		font.setPixelSize(fontsize);
		font.setStyleStrategy(QFont::PreferAntialias);
		font.setStyleStrategy(QFont::PreferQuality);

		painter.setRenderHint(QPainter::TextAntialiasing);
		painter.setFont(font);
		painter.setPen(Qt::white);
		int range = 2;
		for(int y = -range; y <= range; ++y){
			for(int x = -range; x <= range; ++x){
				painter.drawText(QRectF(p1 + QPointF(x, y), p2), text, flags);
			}
		}
		painter.setPen(Qt::black);
		painter.drawText(QRectF(p1, p2), text, flags);
	}

	void slider::wheelEvent(QWheelEvent* event){
		auto step_pos = colors_.step_pos(shift_);
		double factor = 1;
		if(event->buttons() & Qt::LeftButton) factor = 0.25;
		if(event->buttons() & Qt::RightButton) factor = 4;
		step_pos = inc_step(
			step_pos,
			std::max(factor * colors_.strips() / 32, 1.) *
			(event->angleDelta().y() > 0 ? 1 : -1)
		);
		set_shift(colors_.pass_step_pos(step_pos));
	}


}
