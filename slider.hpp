#ifndef _bitmap_viewer_slider_hpp_INCLUDED_
#define _bitmap_viewer_slider_hpp_INCLUDED_

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
	void set_shift(unsigned);

	void contrast_line(bool enable);

	void next();

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
