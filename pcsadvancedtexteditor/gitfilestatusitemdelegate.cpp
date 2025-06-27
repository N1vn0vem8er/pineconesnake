#include "gitfilestatusitemdelegate.h"
#include <qapplication.h>

GitFileStatusItemDelegate::GitFileStatusItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void GitFileStatusItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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
        QApplication::style()->drawItemText(painter, opt.rect, Qt::AlignLeft, opt.palette, opt.state & QStyle::State_Enabled, opt.text);
        break;
    }
}

QSize GitFileStatusItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

bool GitFileStatusItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return true;
}
