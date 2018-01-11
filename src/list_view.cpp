//-----------------------------------------------------------------------------
// Copyright (c) 2017-2018 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <BitmapViewer/list_view.hpp>


namespace bitmap_viewer{


	void list_view::currentChanged(
		QModelIndex const& current,
		QModelIndex const& previous
	){
		QListView::currentChanged(current, previous);
		activated(current);
	}


}
