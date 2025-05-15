#ifndef NOWPLAYINGWIDGET_H
#define NOWPLAYINGWIDGET_H

#include "structs.h"
#include <QWidget>

namespace Ui {
class NowPlayingWidget;
}

class NowPlayingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NowPlayingWidget(QWidget *parent = nullptr);
    ~NowPlayingWidget();

public slots:
    void setTrack(const Track& track);

private:
    Ui::NowPlayingWidget *ui;
    Track track;

private slots:
    void madeFavorite();

signals:
    void makeFavorite(const Track& track);
};

#endif // NOWPLAYINGWIDGET_H
