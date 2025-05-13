#include "playlistmodel.h"
#include "qfileinfo.h"

PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractListModel(parent)
{

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
        return items.at(index.row()).title.isEmpty() ? QFileInfo(items.at(index.row()).path).fileName() : items.at(index.row()).title;
    }
    if(index.column() == 1 && role == Qt::DisplayRole)
    {
        return items.at(index.row()).artist;
    }
    return QVariant();
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

void PlaylistModel::setItems(const QList<Track> &items)
{
    this->items = items;
}
