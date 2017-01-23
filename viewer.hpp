#ifndef _bitmap_viewer_viewer_hpp_INCLUDED_
#define _bitmap_viewer_viewer_hpp_INCLUDED_

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

public slots:
	void next_mode();
	void logarithm(bool logarithm);

signals:
	void info(QString);

protected:
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void paintEvent(QPaintEvent* event);

private:
	item const* item_;
	slider const* slider_;
	mode mode_;
	bool logarithm_;
};


}


#endif
