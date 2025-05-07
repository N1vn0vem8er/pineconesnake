#ifndef FAVORITEITEMWIDGET_H
#define FAVORITEITEMWIDGET_H

#include <QStyledItemDelegate>

class FavoriteItemWidget : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FavoriteItemWidget(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void playPressed(int index);
    void addToPlaylist(int index);
    void unFavorite(int index);
};

#endif // FAVORITEITEMWIDGET_H
