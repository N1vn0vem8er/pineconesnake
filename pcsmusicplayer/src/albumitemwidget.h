#ifndef ALBUMITEMWIDGET_H
#define ALBUMITEMWIDGET_H

#include "qwidget.h"
#include "structs.h"

class AlbumItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlbumItemWidget(const QString title, QWidget *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QString title;

private slots:
    void buttonClicked();

signals:
    void doubleClicked(const QString& title);
    void addToPlaylist(const QList<Track> tracks);
};

#endif // ALBUMITEMWIDGET_H
