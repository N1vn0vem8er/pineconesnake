#include "playingwidget.h"
#include "qfileinfo.h"
#include "ui_playingwidget.h"
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QDateTime>

PlayingWidget::PlayingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayingWidget)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput;
    audioOutput->setVolume(100);
    connect(ui->pauseButton, &QPushButton::clicked, player, &QMediaPlayer::pause);
    connect(ui->playButton, &QPushButton::clicked, player, &QMediaPlayer::play);
}

PlayingWidget::~PlayingWidget()
{
    delete ui;
}

void PlayingWidget::play(const Track &track)
{
    this->track = track;
    player->setAudioOutput(audioOutput);
    ui->titleLabel->setText(this->track.title.isEmpty() ? QFileInfo(this->track.path).fileName() : this->track.title);
    ui->durationLabel->setText(QString("%1:%2:%3").arg((this->track.length / 60 / 60) % 60).arg((this->track.length / 60) % 60).arg(this->track.length % 60));
    player->setSource(QUrl(this->track.path));
    player->play();
}
