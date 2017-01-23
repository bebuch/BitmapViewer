#include "slider.hpp"

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
	startpos_(0){
	setCursor(Qt::OpenHandCursor);
	colors.update.connect([this]{repaint();change();});
}

void slider::set_shift(unsigned s){
	shift_ = s;
	repaint();
	shift_changed(s);
	change();
}

void slider::contrast_line(bool enable){
	colors.contrast_line(enable);
}

void slider::next(){
	colors.next();
}

void slider::right_shift(){
	set_shift(shift_ - shift_range / 100);
}

void slider::left_shift(){
	set_shift(shift_ + shift_range / 100);
}

void slider::fold_linear(){
	colors.set_fold(colors.fold() + 1);
}

void slider::unfold_linear(){
	colors.set_fold(colors.fold() - 1);
}

void slider::fold_exponential(){
	colors.set_fold(colors.fold() << 1);
}

void slider::unfold_exponential(){
	colors.set_fold(colors.fold() >> 1);
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
		painter.setPen(colors(shift_ + i * (shift_range / w)));
		painter.drawLine(i, 0, i, height());
	}
}

void slider::wheelEvent(QWheelEvent* event){
	double factor = 2;
	if(event->buttons() & Qt::LeftButton) factor = 32;
	if(event->buttons() & Qt::RightButton) factor = 0.5;
	auto add = event->angleDelta().y() * (shift_range / (360 * 8 * factor));
	startshift_ += add;
	set_shift(shift() + add);
}


}
