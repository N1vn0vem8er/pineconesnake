#ifndef FILESLISTMODEL_H
#define FILESLISTMODEL_H

#include "structs.h"
#include <QAbstractListModel>

class FilesListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FilesListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    void setItems(const QList<Track> items);

private:
    QList<Track> items;
};

#endif // FILESLISTMODEL_H
