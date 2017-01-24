//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <BitmapViewer/main_window.hpp>

#include <QApplication>


int main(int argc, char *argv[]){
	QApplication a(argc, argv);

	bitmap_viewer::main_window w;
	w.show();

	QStringList list;
	for(auto const& filename: a.arguments()){
		if(filename == a.arguments()[0]) continue;
		if(filename.size() == 0 || filename[0] != '/') continue;
		list.append(filename);
	}
	w.load_files(list);

	return a.exec();
}
