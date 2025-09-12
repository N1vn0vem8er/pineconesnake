#include "searchresultdelegate.h"
#include <QPainter>
#include <qpdfsearchmodel.h>

SearchResultDelegate::SearchResultDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void SearchResultDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QString displayText = index.data().toString();
    const auto boldBegin = displayText.indexOf(u"<b>", 0, Qt::CaseInsensitive) + 3;
    const auto boldEnd = displayText.indexOf(u"</b>", boldBegin, Qt::CaseInsensitive);
    if (boldBegin >= 3 && boldEnd > boldBegin) {
        const QString pageLabel = tr("Page %1: ").arg(index.data(int(QPdfSearchModel::Role::Page)).toInt() + 1);
        const QString boldText = displayText.mid(boldBegin, boldEnd - boldBegin);
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());
        const QFont defaultFont = painter->font();
        QFontMetrics fm = painter->fontMetrics();
        auto pageLabelWidth = fm.horizontalAdvance(pageLabel);
        const int offset = (option.rect.height() - fm.height()) / 2 + fm.ascent();
        painter->drawText(0, option.rect.y() + offset, pageLabel);
        QFont boldFont = defaultFont;
        boldFont.setBold(true);
        int boldWidth = QFontMetrics(boldFont).horizontalAdvance(boldText);
        int prefixSuffixWidth = (option.rect.width() - pageLabelWidth - boldWidth) / 2;
        painter->setFont(boldFont);
        painter->drawText(pageLabelWidth + prefixSuffixWidth, option.rect.y() + offset, boldText);
        painter->setFont(defaultFont);
        const QString suffix = fm.elidedText(displayText.mid(boldEnd + 4), Qt::ElideRight, prefixSuffixWidth);
        painter->drawText(pageLabelWidth + prefixSuffixWidth + boldWidth, option.rect.y() + offset, suffix);
        const QString prefix = fm.elidedText(displayText.left(boldBegin - 3), Qt::ElideLeft, prefixSuffixWidth);
        painter->drawText(pageLabelWidth + prefixSuffixWidth - fm.horizontalAdvance(prefix), option.rect.y() + offset, prefix);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}
