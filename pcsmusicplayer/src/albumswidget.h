#ifndef ALBUMSWIDGET_H
#define ALBUMSWIDGET_H

#include "qgridlayout.h"
#include "trackitemwidget.h"
#include "tracklistmodel.h"
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
    TrackListModel* model = nullptr;
    TrackItemWidget* delegate = nullptr;
    QList<Track> tracks;

private slots:
    void openAlbum(const QString& title);
    void backPressed();
    void playPressed(int index);
    void addToPlaylistPressed(int index);

signals:
    void play(const Track& track);
    void addToPlaylist(const Track& track);
};

#endif // ALBUMSWIDGET_H
