#include "main_window.hpp"

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
