//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _bitmap_viewer__list_model__hpp_INCLUDED_
#define _bitmap_viewer__list_model__hpp_INCLUDED_

#include "item.hpp"

#include <QAbstractListModel>


namespace bitmap_viewer{


	class list_model: public QAbstractListModel{
		Q_OBJECT

	public:
		explicit list_model(QObject *parent = 0);

		virtual QVariant headerData(
			int section,
			Qt::Orientation orientation,
			int role = Qt::DisplayRole
		)const;

		virtual int rowCount(QModelIndex const& parent = QModelIndex())const;

		virtual QVariant data(
			QModelIndex const& index,
			int role = Qt::DisplayRole
		)const;

		virtual bool setData(
			QModelIndex const& index,
			QVariant const& value,
			int role = Qt::EditRole
		);
		virtual Qt::ItemFlags flags(QModelIndex const& index)const;

		virtual bool insertRows(
			int row,
			int count,
			QModelIndex const& parent = QModelIndex()
		);
		virtual bool removeRows(
			int row,
			int count,
			QModelIndex const& parent = QModelIndex()
		);

		void append_files(QList< item > const& list);

		void close_selected(QModelIndexList const& index_list);

		item* get(std::size_t index);

	private:
		QList< item > items_;
	};


}


#endif
