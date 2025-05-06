#include "fileslistmodel.h"

#include <QFileInfo>

FilesListModel::FilesListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int FilesListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : items.count();
}

int FilesListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant FilesListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= items.count())
    {
        return QVariant();
    }
    if(role == Qt::DisplayRole)
        switch(index.column())
        {
        case 0:
            return items.at(index.row()).title.isEmpty() ? QFileInfo(items.at(index.row()).path).fileName() : items.at(index.row()).title;
            break;
        case 1:
            return items.at(index.row()).path;
            break;
        }
    return QVariant();
}

void FilesListModel::setItems(const QList<Track> items)
{
    this->items = items;
}
