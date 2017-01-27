//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <BitmapViewer/list_model.hpp>


namespace bitmap_viewer{


	list_model::list_model(QObject *parent):
		QAbstractListModel(parent)
		{}

	int list_model::rowCount(QModelIndex const&)const{
		return bitmaps_.size();
	}

	QVariant list_model::data(QModelIndex const& index, int role)const{
		if(!index.isValid()) return QVariant();

		if(role == Qt::DisplayRole){
			auto const& info = bitmaps_.at(index.row());
			return
				info.filename() + "\n" +
				QString(tr("%1\n")).arg(info.type()) +
				QString(tr("%1 x %2 Pixel\n"))
					.arg(info.width()).arg(info.height()) +
				(info.undefined_count() ? QString(tr("%2 Undefinded\n"))
					.arg(info.undefined_count()) : QString()) +
				QString(tr("%1 – %2")).arg(info.min()).arg(info.max());
		}

		if(role == Qt::DecorationRole){
			return bitmaps_.at(index.row()).icon();
		}

		if(role == Qt::ToolTipRole){
			return bitmaps_.at(index.row()).path_and_file();
		}

		if(role == Qt::UserRole){
			QVariant result;
			result.setValue(bitmaps_.at(index.row()));
			return result;
		}

		return QVariant();
	}

	QVariant list_model::headerData(
		int section, Qt::Orientation orientation, int role
	)const{
		if(role != Qt::DisplayRole) return QVariant();

		if(orientation == Qt::Horizontal){
			switch(section){
			case 0: return tr("Bitmap");
			default: return QVariant();
			}
		}else{
			return QString("%1").arg(section);
		}
	}

	bool list_model::setData(
		QModelIndex const& index, QVariant const& value, int role
	){
		if(index.isValid() && role == Qt::EditRole){
			int row = index.row();
			auto bitmap = value.value< item >();
			bitmaps_.replace(row, bitmap);

			emit(dataChanged(index, index));
			return true;
		}

		return false;
	}

	Qt::ItemFlags list_model::flags(QModelIndex const& index)const{
		if(!index.isValid()) return Qt::ItemIsEnabled;
		return QAbstractListModel::flags(index) | Qt::ItemIsSelectable;
	}

	bool list_model::insertRows(int row, int count, QModelIndex const& parent){
		beginInsertRows(parent, row, row + count - 1);

		for(int i = 0; i < count; ++i){
			bitmaps_.insert(row, item());
		}

		endInsertRows();
		return true;
	}

	bool list_model::removeRows(int row, int count, QModelIndex const& parent){
		beginRemoveRows(parent, row, row + count - 1);

		for(int i = 0; i < count; ++i){
			bitmaps_.removeAt(row);
		}

		endRemoveRows();
		return true;
	}

	void list_model::append_files(QList< item > const& list){
		auto count = bitmaps_.size();
		if(!insertRows(count, list.size())) return;

		for(int i = 0; i < list.size(); ++i){
			QVariant var;
			var.setValue(list.at(i));
			setData(index(count + i), var);
		}
	}

	void list_model::close_selected(QModelIndexList const& index_list){
		int i = 0;
		for(auto const& index: index_list){
			removeRows(index.row() - i, 1);
			++i;
		}
	}

	item const* list_model::get(std::size_t index)const{
		return &bitmaps_.at(index);
	}



}
