#ifndef _bitmap_viewer_list_model_hpp_INCLUDED_
#define _bitmap_viewer_list_model_hpp_INCLUDED_

#include "item.hpp"

#include <QAbstractListModel>


namespace bitmap_viewer{


class list_model: public QAbstractListModel{
	Q_OBJECT

public:
	explicit list_model(QObject *parent = 0);

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)const;
	virtual int rowCount(QModelIndex const& parent = QModelIndex())const;
	virtual QVariant data(QModelIndex const& index, int role = Qt::DisplayRole)const;

	virtual bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole);
	virtual Qt::ItemFlags flags(QModelIndex const& index)const;

	virtual bool insertRows(int row, int count, QModelIndex const& parent = QModelIndex());
	virtual bool removeRows(int row, int count, QModelIndex const& parent = QModelIndex());

	void append_files(QList< item > const& list);

	void close_selected(QModelIndexList const& index_list);

	item const* get(std::size_t index)const;

private:
	QList< item > bitmaps_;
};


}


#endif
