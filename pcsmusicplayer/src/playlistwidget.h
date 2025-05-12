#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include "playlistitem.h"
#include "playlistmodel.h"
#include "structs.h"

#include <QWidget>

namespace Ui {
class PlaylistWidget;
}

class PlaylistWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistWidget(QWidget *parent = nullptr);
    explicit PlaylistWidget(QList<Track> tracks, QWidget *parent = nullptr);
    ~PlaylistWidget();
    void loadTracks(const QList<Track> &tracks);

public slots:
    void playingTrack(const Track& track);
    void addTrack(const Track& track);
    void trackFinished(const Track& track);

private:
    Ui::PlaylistWidget *ui;
    QList<Track> tracks;
    PlaylistItem* delegate = nullptr;
    PlaylistModel* model = nullptr;
    int playingIndex = 0;
    void init();

private slots:
    void removeTrack(const Track& track);
    void playTrack(int index);
    void removeTrack(int index);
    void clear();
    void savePlaylist();
    void playlistName(const QString& name);

signals:
    void trackSelected(const Track& track);
    void playTrack(const Track& track);
    void playlistSaved();

};

#endif // PLAYLISTWIDGET_H
