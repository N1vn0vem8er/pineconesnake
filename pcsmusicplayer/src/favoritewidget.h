#ifndef FAVORITEWIDGET_H
#define FAVORITEWIDGET_H

#include "favoriteitemwidget.h"
#include "favoritemodel.h"
#include <QWidget>

namespace Ui {
class FavoriteWidget;
}

class FavoriteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FavoriteWidget(QWidget *parent = nullptr);
    ~FavoriteWidget();
    void loadFavorites();

private:
    Ui::FavoriteWidget *ui;
    FavoriteItemWidget* delegate = nullptr;
    FavoriteModel* model = nullptr;
    QList<Track> tracks;

private slots:
    void makeFavorite(int index);
    void playPressed(int index);
    void addToPlaylistPressed(int index);

signals:
    void play(const Track& track);
    void addToPlaylist(const Track& track);
};

#endif // FAVORITEWIDGET_H
