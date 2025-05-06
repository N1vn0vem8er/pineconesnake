#ifndef FILESLISTITEMWIDGET_H
#define FILESLISTITEMWIDGET_H

#include <QStyledItemDelegate>

class FilesListItemWidget : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FilesListItemWidget(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void playPressed(int index);
    void addToPlaylist(int index);
};

#endif // FILESLISTITEMWIDGET_H
