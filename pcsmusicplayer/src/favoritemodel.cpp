#include "favoritemodel.h"

#include <QFileInfo>

FavoriteModel::FavoriteModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int FavoriteModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : items.count();
}

int FavoriteModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant FavoriteModel::data(const QModelIndex &index, int role) const
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
            return items.at(index.row()).artist;
        case 2:
            return items.at(index.row()).album;
            break;
        }
    return QVariant();
}

void FavoriteModel::setItems(const QList<Track> items)
{
    this->items = items;
}
