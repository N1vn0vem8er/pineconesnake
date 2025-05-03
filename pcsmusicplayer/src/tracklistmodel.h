#ifndef TRACKLISTMODEL_H
#define TRACKLISTMODEL_H

#include "structs.h"
#include <QAbstractListModel>

class TrackListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TrackListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void setItems(const QList<Track> &newItems);

private:
    QList<Track> items;
};

#endif // TRACKLISTMODEL_H
