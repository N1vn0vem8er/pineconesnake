#ifndef ALBUMSWIDGET_H
#define ALBUMSWIDGET_H

#include "qgridlayout.h"
#include <QWidget>

namespace Ui {
class AlbumsWidget;
}

class AlbumsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AlbumsWidget(QWidget *parent = nullptr);
    ~AlbumsWidget();
    void loadAlbums();

private:
    Ui::AlbumsWidget *ui;
    QStringList albums;
    QList<QWidget*> albumWidgets;
    QGridLayout* layout = nullptr;

private slots:
    void openAlbum(const QString& title);
    void backPressed();
};

#endif // ALBUMSWIDGET_H
