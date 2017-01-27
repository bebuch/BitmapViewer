//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <BitmapViewer/main_window.hpp>
#include <BitmapViewer/list_model.hpp>

#include <QMessageBox>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>


namespace bitmap_viewer{


	main_window::main_window():
		dialog_(this){
		ui.setupUi(this);

		info_ui.setupUi(&info);
		info.setModal(true);
		connect(ui.action_info, SIGNAL(triggered()), &info, SLOT(show()));

		ui.list->setModel(new list_model);
		ui.viewer->set_slider(ui.slider);
		connect(ui.slider, SIGNAL(change()), ui.viewer, SLOT(repaint()));

		connect(
			ui.viewer, SIGNAL(info(QString)),
			ui.status, SLOT(showMessage(QString))
		);
		connect(
			ui.action_logarithm, SIGNAL(triggered(bool)),
			ui.viewer, SLOT(logarithm(bool))
		);
		connect(
			ui.action_next_show_mode, SIGNAL(triggered()),
			ui.viewer, SLOT(next_mode())
		);

		setAcceptDrops(true);
	}

	void main_window::open(){
		dialog_.setAcceptMode(QFileDialog::AcceptOpen);
		dialog_.setOption(QFileDialog::ShowDirsOnly, false);
		dialog_.setFileMode(QFileDialog::ExistingFiles);
		dialog_.setNameFilter(tr("Big Files (*.big);;All Files (*)"));
		if(!dialog_.exec()) return;

		load_files(dialog_.selectedFiles());
	}

	void main_window::save(){
		QModelIndexList indexes = ui.list->selectionModel()->selectedIndexes();
		if(indexes.size() == 0){
			QMessageBox::information(this, tr("No image selected"),
				tr("Please select at least one image."));
		}else if(indexes.size() == 1){
			auto const data = indexes[0].data(Qt::UserRole).value< item >();

			dialog_.setAcceptMode(QFileDialog::AcceptSave);
			dialog_.setOption(QFileDialog::ShowDirsOnly, false);
			dialog_.setFileMode(QFileDialog::AnyFile);
			dialog_.setNameFilter(tr("Portable Network Graphics (*.png)"));
			QMessageBox::information(this, "log", data.path());
			dialog_.setDirectory(data.path());
			if(!dialog_.exec()) return;

			auto const files = dialog_.selectedFiles();
			assert(files.size() == 1);
			auto const filename = files[0];

			save_image(data, filename);
		}else{
			auto const data = indexes[0].data(Qt::UserRole).value< item >();

			dialog_.setAcceptMode(QFileDialog::AcceptSave);
			dialog_.setFileMode(QFileDialog::AnyFile);
			dialog_.setOption(QFileDialog::ShowDirsOnly, true);
			dialog_.setNameFilter(tr("Portable Network Graphics (*.png)"));
			dialog_.setDirectory(data.path());
			if(!dialog_.exec()) return;

			for(auto const& index: indexes){
				auto const data = index.data().value< item >();
				auto filename = data.filename();
				filename = filename.left(filename.lastIndexOf("."));
				save_image(data, filename + ".png");
			}
		}
	}

	void main_window::close_selected(){
		auto model = dynamic_cast< list_model* >(ui.list->model());
		if(!model) return;

		ui.viewer->set_bitmap(nullptr);
		model->close_selected(ui.list->selectionModel()->selectedIndexes());
	}

	void main_window::show_bitmap(QModelIndex index){
		auto model = dynamic_cast< list_model const* >(ui.list->model());
		if(!model) return;

		ui.viewer->set_bitmap(model->get(index.row()));
	}

	void main_window::dragEnterEvent(QDragEnterEvent* event){
		if(!event->mimeData()->hasUrls()) return;
		for(auto const& url: event->mimeData()->urls()){
			if(!url.isLocalFile()) return;
		}
		event->acceptProposedAction();
	}

	void main_window::dropEvent(QDropEvent* event){
		QStringList filenames;
		for(auto const& url: event->mimeData()->urls()){
			filenames.push_back(url.toLocalFile());
		}

		load_files(filenames);

		event->acceptProposedAction();
	}

	void main_window::load_files(QStringList const& filenames){
		if(filenames.isEmpty()) return;

		QList< item > list;
		QStringList errors;
		for(auto const& filename: filenames){
			try{
				item item(filename);
				list.append(item);
			}catch(std::exception const& error){
				errors.push_back(error.what());
			}
		}

		auto* model = dynamic_cast< list_model* >(ui.list->model());
		model->append_files(list);

		ui.list->activated(model->index(model->rowCount() - 1));

		if(errors.isEmpty()) return;

		QMessageBox::warning(this, tr("Loading problems"),
			tr("Could not load all files:\n") + errors.join("\n"));
	}

	void main_window::save_image(item const& item, QString const& filename){

	}


}
