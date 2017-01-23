#ifndef _bitmap_viewer_item_hpp_INCLUDED_
#define _bitmap_viewer_item_hpp_INCLUDED_

#include "file_handler.hpp"
#include "colors.hpp"

#include <QImage>
#include <QPixmap>
#include <QFileInfo>


namespace bitmap_viewer{


class item{
public:
	item();
	item(QString const& filename);

	QString filename()const;
	QString path()const;

	QString type()const;

	QString min()const;
	QString max()const;

	std::size_t width()const;
	std::size_t height()const;

	std::size_t undefined_count()const;

	QImage image(colors const& color, unsigned shift, bool logarithm)const;

	QPixmap icon()const;

	bitmap_type const& bitmap()const{return bitmap_;}

private:
	bitmap_type bitmap_;

	QFileInfo filename_;

	QPixmap icon_;
};


}

Q_DECLARE_METATYPE(bitmap_viewer::item)


#endif
