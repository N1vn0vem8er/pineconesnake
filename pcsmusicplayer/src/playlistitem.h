#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <QStyledItemDelegate>

class PlaylistItem : public QStyledItemDelegate
{
    Q_OBJECT
public:
    PlaylistItem(QObject* parent = nullptr);

public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
signals:
    void playPressed(int index);
    void removePressed(int index);
};

#endif // PLAYLISTITEM_H
