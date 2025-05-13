#include "playlistitem.h"
#include "qapplication.h"

#include <QMouseEvent>
#include <QPainter>

PlaylistItem::PlaylistItem(QObject *parent) : QStyledItemDelegate(parent)
{

}

void PlaylistItem::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    switch(index.column())
    {
    case 0:
    case 1:
        QStyledItemDelegate::paint(painter, opt, index);
        break;
    case 2:
    case 3:
        QStyleOptionButton button;
        button.rect = QRect(opt.rect.right() - 32,
                            opt.rect.top() + (opt.rect.height() - 32) / 2,
                            32, 32);
        button.state = QStyle::State_Enabled;
        button.icon = (index.column() == 2) ? QIcon::fromTheme("media-playback-start") : QIcon::fromTheme("edit-delete");
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
        break;
    }
}

QSize PlaylistItem::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 0) {
        return QStyledItemDelegate::sizeHint(option, index);
    } else {
        return QSize(100, 30);
    }
}

bool PlaylistItem::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
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
            emit removePressed(index.row());
        }
    }
    return true;
}
