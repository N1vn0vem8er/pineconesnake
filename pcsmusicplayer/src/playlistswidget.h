#ifndef PLAYLISTSWIDGET_H
#define PLAYLISTSWIDGET_H

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
};

#endif // PLAYLISTSWIDGET_H
