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

private slots:
    void diplayPostion(qint64 position);
    void positionChanged();
    void setVolume(float value);

signals:
    void trackFinished(const Track& track);
    void playNext();
    void playPreviout();
    void playingTrack(const Track& track);
};

#endif // PLAYINGWIDGET_H
