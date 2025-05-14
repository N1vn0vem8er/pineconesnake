#include "trackitemwidget.h"

#include <QMouseEvent>
#include <QApplication>

TrackItemWidget::TrackItemWidget(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void TrackItemWidget::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    QStyleOptionButton button;
    button.rect = QRect(opt.rect.right() - 32,
                        opt.rect.top() + (opt.rect.height() - 32) / 2,
                        32, 32);
    button.state = QStyle::State_Enabled;
    switch(index.column())
    {
    case 0:
    case 1:
    case 2:
    case 3:
        QStyledItemDelegate::paint(painter, opt, index);
        break;
    case 4:
        button.icon = QIcon::fromTheme("media-playback-start");
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
        break;
    case 5:
        button.icon = QIcon::fromTheme("list-add");
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
        break;
    case 6:
        button.icon = QIcon::fromTheme("emblem-favorite");
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
        break;
    }
}

QSize TrackItemWidget::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 0) {
        return QStyledItemDelegate::sizeHint(option, index);
    } else {
        return QSize(100, 30);
    }
}

bool TrackItemWidget::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* e = (QMouseEvent*) event;
        QRect r = option.rect;
        int x = r.right() - 32;
        int y = r.top() + (r.height() - 32)/2;
        int w = 32;
        int h = 32;

        if(index.column() == 4 && e->pos().x() > x && e->pos().x() < x + w && e->pos().y() > y && e->pos().y() < y + h)
        {
            emit playPressed(index.row());
        }
        else if(index.column() == 5 && e->pos().x() > x && e->pos().x() < x + w && e->pos().y() > y && e->pos().y() < y + h)
        {
            emit addToPlaylist(index.row());
        }
        else if(index.column() == 6 && e->pos().x() > x && e->pos().x() < x + w && e->pos().y() > y && e->pos().y() < y + h)
        {
            emit makeFavorite(index.row());
        }
    }
    return true;
}
