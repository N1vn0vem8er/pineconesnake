#include "favoriteitemwidget.h"
#include "qevent.h"

#include <QApplication>

FavoriteItemWidget::FavoriteItemWidget(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void FavoriteItemWidget::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    QStyleOptionButton button;
    switch(index.column())
    {
    case 0:
    case 1:
    case 2:
        QStyledItemDelegate::paint(painter, opt, index);
        break;
    case 3:
        button.icon = QIcon::fromTheme("media-playback-start");
        button.rect = QRect(opt.rect.right() - 32,
                            opt.rect.top() + (opt.rect.height() - 32) / 2,
                            32, 32);
        button.state = QStyle::State_Enabled;
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
        break;
    case 4:
        button.rect = QRect(opt.rect.right() - 32,
                            opt.rect.top() + (opt.rect.height() - 32) / 2,
                            32, 32);
        button.state = QStyle::State_Enabled;
        button.icon = QIcon::fromTheme("list-add");
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
        break;
    case 5:
        button.rect = QRect(opt.rect.right() - 32,
                            opt.rect.top() + (opt.rect.height() - 32) / 2,
                            32, 32);
        button.state = QStyle::State_Enabled;
        button.icon = QIcon::fromTheme("emblem-favorite");
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
        break;
    }
}

QSize FavoriteItemWidget::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 0) {
        return QStyledItemDelegate::sizeHint(option, index);
    } else {
        return QSize(100, 30);
    }
}

bool FavoriteItemWidget::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* e = (QMouseEvent*) event;
        QRect r = option.rect;
        int x = r.right() - 32;
        int y = r.top() + (r.height() - 32)/2;
        int w = 32;
        int h = 32;

        if(index.column() == 3 && e->pos().x() > x && e->pos().x() < x + w && e->pos().y() > y && e->pos().y() < y + h)
        {
            emit playPressed(index.row());
        }
        else if(index.column() == 4 && e->pos().x() > x && e->pos().x() < x + w && e->pos().y() > y && e->pos().y() < y + h)
        {
            emit addToPlaylist(index.row());
        }
        else if(index.column() == 5 && e->pos().x() > x && e->pos().x() < x + w && e->pos().y() > y && e->pos().y() < y + h)
        {
            emit unFavorite(index.row());
        }
    }
    return true;
}
