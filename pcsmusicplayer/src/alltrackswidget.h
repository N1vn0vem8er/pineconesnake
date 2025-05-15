#ifndef ALLTRACKSWIDGET_H
#define ALLTRACKSWIDGET_H

#include "trackitemwidget.h"
#include "tracklistmodel.h"
#include <QWidget>

namespace Ui {
class AllTracksWidget;
}

class AllTracksWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AllTracksWidget(QWidget *parent = nullptr);
    ~AllTracksWidget();
    void loadTracks();

private:
    Ui::AllTracksWidget *ui;
    TrackItemWidget* delegate = nullptr;
    TrackListModel* model = nullptr;
    QList<Track> tracks;
    void displayTracks();
    void search();

private slots:
    void addToPlayList(int index);
    void playPressed(int index);
    void sort(int index);
    void madeFavorite(int index);

signals:
    void addToPlaylist(const Track& track);
    void play(const Track& track);
    void makeFavorite(const Track& track);
};

#endif // ALLTRACKSWIDGET_H
