#include "playlistmodel.h"

PlaylistModel::PlaylistModel(QStringList items, QObject *parent)
    : QAbstractListModel(parent)
{
    this->items = items;
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : items.count();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= items.count())
    {
        return QVariant();
    }
    if(index.column() == 0 && role == Qt::DisplayRole)
    {
        return items.at(index.row());
    }
    if(index.column() == 1 && role == Qt::DisplayRole)
    {
        return items.at(index.row());
    }
    return QVariant();
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}
