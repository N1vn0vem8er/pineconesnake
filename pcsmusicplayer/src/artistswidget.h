#ifndef ARTISTSWIDGET_H
#define ARTISTSWIDGET_H

#include "qgridlayout.h"
#include "structs.h"
#include "trackitemwidget.h"
#include "tracklistmodel.h"
#include <QWidget>

namespace Ui {
class ArtistsWidget;
}

class ArtistsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ArtistsWidget(QWidget *parent = nullptr);
    ~ArtistsWidget();
    void loadArtists();

private:
    Ui::ArtistsWidget *ui;
    QStringList artists;
    TrackItemWidget *delegate;
    QList<Track> tracks;
    QList<QWidget*> artistWidgets;
    QGridLayout* layout = nullptr;
    TrackListModel* model = nullptr;

private slots:
    void backPressed();
    void addToPlaylistPressed(int index);
    void playPressed(int index);
    void openTracks(const QString& title);
    void madeFavorite(int index);

signals:
    void play(const Track& track);
    void addToPlaylist(const Track& track);
    void makeFavorite(const Track& track);
};

#endif // ARTISTSWIDGET_H
