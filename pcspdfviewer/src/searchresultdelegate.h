#ifndef SEARCHRESULTDELEGATE_H
#define SEARCHRESULTDELEGATE_H

#include <QStyledItemDelegate>

class SearchResultDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SearchResultDelegate(QObject* parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // SEARCHRESULTDELEGATE_H
