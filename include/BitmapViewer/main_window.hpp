#ifndef _bitmap_viewer_mainwindow_hpp_INCLUDED_
#define _bitmap_viewer_mainwindow_hpp_INCLUDED_

#include "ui_main_window.h"
#include "ui_info.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QDialog>


namespace bitmap_viewer{


class main_window: public QMainWindow{
	Q_OBJECT

public:
	main_window();

public slots:
	void open();
	void save();
	void close_selected();
	void show_bitmap(QModelIndex index);
	void load_files(QStringList const& filenames);

protected:
	virtual void dragEnterEvent(QDragEnterEvent* event);
	virtual void dropEvent(QDropEvent* event);

private:
	QDialog info;

	QFileDialog dialog_;

	Ui::main_window ui;
	Ui::info info_ui;
};


}


#endif
