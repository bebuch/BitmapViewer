#include "viewer.hpp"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

#include <sstream>


namespace bitmap_viewer{


viewer::viewer():
	item_(nullptr),
	slider_(nullptr),
	mode_(mode::pass),
	logarithm_(false)
{
	setMouseTracking(true);
}

void viewer::set_bitmap(item const* i){
	item_ = i;
	if(mode_ == mode::scroll && item_) setMinimumSize(item_->width(), item_->height());
	repaint();
}

void viewer::set_slider(slider const* s){
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

void viewer::logarithm(bool logarithm){
	logarithm_ = logarithm;
	repaint();
}

template < typename T >
QString toQString(T const& v){
	std::ostringstream os;
	os << v;
	return os.str().c_str();
}

class print_value: public boost::static_visitor< QString >{
public:
	print_value(tools::point_2d< std::size_t > const& point):point(point){}

	template < typename T >
	QString operator()(T const& info)const{
		if(!is_point_in_range(point, info.bitmap)) return QString();
		return QString(QObject::tr("Position: %1, %2; Value: %3"))
			.arg(point.x())
			.arg(point.y())
			.arg(toQString(info.bitmap(point)));
	}

private:
	tools::point_2d< std::size_t > const point;
};

void viewer::mouseMoveEvent(QMouseEvent* event){
	if(!slider_ || !item_) return;

	auto point = event->localPos();

	double iw = item_->width();
	double ih = item_->height();
	double factor = std::max(iw / width(), ih / height());
	if(mode_ != mode::scale && factor < 1) factor = 1;
	point *= factor;

	auto p = tools::make_point< std::size_t >(static_cast< std::size_t >(point.x()), static_cast< std::size_t >(point.y()));
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
	painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
	painter.setPen(Qt::NoPen);
	painter.setBrush(slider_->colors.brush());
	painter.drawRect(rect);
	painter.drawImage(rect, item_->image(slider_->colors, slider_->shift(), logarithm_));
}


}
