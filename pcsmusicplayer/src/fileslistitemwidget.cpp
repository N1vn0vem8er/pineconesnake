#include "fileslistitemwidget.h"
#include "qcoreevent.h"
#include "qevent.h"

#include <QApplication>

FilesListItemWidget::FilesListItemWidget(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void FilesListItemWidget::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    if (index.column() == 0 || index.column() == 1) {
        QStyledItemDelegate::paint(painter, opt, index);
    } else if (index.column() == 2 || index.column() == 3) {
        QStyleOptionButton button;
        button.rect = QRect(opt.rect.right() - 32,
                            opt.rect.top() + (opt.rect.height() - 32) / 2,
                            32, 32);
        button.state = QStyle::State_Enabled;
        button.icon = (index.column() == 2) ? QIcon::fromTheme("media-playback-start") : QIcon::fromTheme("list-add");
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
    }
}

QSize FilesListItemWidget::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 0) {
        return QStyledItemDelegate::sizeHint(option, index);
    } else {
        return QSize(100, 30);
    }
}

bool FilesListItemWidget::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* e = (QMouseEvent*) event;
        QRect r = option.rect;
        int x = r.right() - 32;
        int y = r.top() + (r.height() - 32)/2;
        int w = 32;
        int h = 32;

        if(index.column() == 2 && e->pos().x() > x && e->pos().x() < x + w && e->pos().y() > y && e->pos().y() < y + h)
        {
            emit playPressed(index.row());
        }
        else if(index.column() == 3 && e->pos().x() > x && e->pos().x() < x + w && e->pos().y() > y && e->pos().y() < y + h)
        {
            emit addToPlaylist(index.row());
        }
    }
    return true;
}
