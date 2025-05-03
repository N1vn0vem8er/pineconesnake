#ifndef TRACKITEMWIDGET_H
#define TRACKITEMWIDGET_H

#include <QStyledItemDelegate>

class TrackItemWidget : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TrackItemWidget(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
signals:
    void addToPlaylist(int index);
    void playPressed(int index);
};

#endif // TRACKITEMWIDGET_H
