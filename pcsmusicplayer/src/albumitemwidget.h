#ifndef ALBUMITEMWIDGET_H
#define ALBUMITEMWIDGET_H

#include "qwidget.h"

class AlbumItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlbumItemWidget(const QString title, QWidget *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QString title;

signals:
    void doubleClicked(const QString& title);
};

#endif // ALBUMITEMWIDGET_H
