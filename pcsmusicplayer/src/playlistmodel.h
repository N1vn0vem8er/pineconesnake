#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include "structs.h"
#include <QAbstractListModel>

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PlaylistModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int columnCount(const QModelIndex &parent) const override;
    void setItems(const QList<Track>& items);

private:
    QList<Track> items;
};

#endif // PLAYLISTMODEL_H
