#ifndef PLAYINGWIDGET_H
#define PLAYINGWIDGET_H

#include "qmediaplayer.h"
#include "structs.h"
#include <QWidget>

namespace Ui {
class PlayingWidget;
}

class PlayingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayingWidget(QWidget *parent = nullptr);
    ~PlayingWidget();

public slots:
    void play(const Track& track);

private:
    Ui::PlayingWidget *ui;
    Track track;
    QMediaPlayer* player = nullptr;
    QAudioOutput* audioOutput = nullptr;
};

#endif // PLAYINGWIDGET_H
