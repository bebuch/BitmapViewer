//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _bitmap_viewer__item__hpp_INCLUDED_
#define _bitmap_viewer__item__hpp_INCLUDED_

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
		QString path_and_file()const;

		QString type()const;

		QString min()const;
		QString max()const;

		std::tuple< double, double > minmax_values()const;
		std::tuple< double, double > minmax_type_values()const;

		std::size_t width()const;
		std::size_t height()const;

		std::size_t undefined_count()const;

		bool is_float()const;

		QImage image(colors const& color, unsigned shift)const;

		QPixmap icon()const;

		bitmap_type const& bitmap()const{return bitmap_;}

	private:
		bitmap_type bitmap_;

		QFileInfo filename_;

		QPixmap icon_;
	};


}

Q_DECLARE_METATYPE(bitmap_viewer::item)
Q_DECLARE_METATYPE(bitmap_viewer::item const*)


#endif
