//-----------------------------------------------------------------------------
// Copyright (c) 2017-2018 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _bitmap_viewer__list_view__hpp_INCLUDED_
#define _bitmap_viewer__list_view__hpp_INCLUDED_

#include <QListView>


namespace bitmap_viewer{


	class list_view: public QListView{
		Q_OBJECT
	public:
		using QListView::QListView;

	protected:
		virtual void currentChanged(
			QModelIndex const& current,
			QModelIndex const& previous
		)override;
	};


}


#endif
