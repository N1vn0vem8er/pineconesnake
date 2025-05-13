#ifndef PLAYLISTSWIDGET_H
#define PLAYLISTSWIDGET_H

#include "playlistitem.h"
#include "playlistmodel.h"
#include "qgridlayout.h"
#include "structs.h"
#include <QWidget>

namespace Ui {
class PlaylistsWidget;
}

class PlaylistsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistsWidget(QWidget *parent = nullptr);
    ~PlaylistsWidget();
    void loadPlaylists();

private:
    Ui::PlaylistsWidget *ui;
    Playlist playlist;
    QStringList playlistNames;
    QGridLayout* layout = nullptr;
    QList<QWidget*> playlistWidgets;
    PlaylistModel* model = nullptr;
    PlaylistItem* delegate = nullptr;

private slots:
    void playlistSelected(const QString& name);
    void playPressed(int index);
    void addToPlaylistPressed(int index);
    void playPlaylist(const QString& name);

signals:
    void play(const Track& track);
    void playlistPlay(const Playlist& playlist);
};

#endif // PLAYLISTSWIDGET_H
