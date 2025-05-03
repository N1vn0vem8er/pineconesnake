#include "tracklistmodel.h"

TrackListModel::TrackListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int TrackListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : items.count();
}

int TrackListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant TrackListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= items.count())
    {
        return QVariant();
    }
    if(role == Qt::DisplayRole)
        switch(index.column())
        {
        case 0:
            return items.at(index.row()).title;
            break;
        case 1:
            return items.at(index.row()).artist;
            break;
        case 2:
            return items.at(index.row()).album;
            break;
        }
    return QVariant();
}

void TrackListModel::setItems(const QList<Track> &newItems)
{
    items = newItems;
}
