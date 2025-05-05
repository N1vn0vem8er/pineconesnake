#include "playingwidget.h"
#include "qfileinfo.h"
#include "settings.h"
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
    connect(player, &QMediaPlayer::positionChanged, this, &PlayingWidget::diplayPostion);
    connect(ui->volume, &QSlider::valueChanged, this, &PlayingWidget::setVolume);
    connect(ui->durationSlider, &QSlider::sliderReleased, this, &PlayingWidget::positionChanged);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [&](QMediaPlayer::MediaStatus status){if(status == QMediaPlayer::MediaStatus::EndOfMedia) emit trackFinished(track);});
    Settings s;
    s.loadSettings();
    setVolume(Settings::volume * 100);
    ui->volume->setValue(Settings::volume * 100);
}

PlayingWidget::~PlayingWidget()
{
    delete audioOutput;
    delete ui;
}

void PlayingWidget::play(const Track &track)
{
    this->track = track;
    player->setAudioOutput(audioOutput);
    ui->titleLabel->setText(this->track.title.isEmpty() ? QFileInfo(this->track.path).fileName() : this->track.title);
    ui->durationLabel->setText(QString("%1:%2:%3").arg((this->track.length / 60 / 60) % 60).arg((this->track.length / 60) % 60).arg(this->track.length % 60));
    ui->durationSlider->setMaximum(this->track.length);
    player->setSource(QUrl(this->track.path));
    player->play();
}

void PlayingWidget::diplayPostion(qint64)
{
    auto tmp = player->position() / 3600;
    ui->durationSlider->setValue(tmp);
    ui->timePassedLabel->setText(QString("%1:%2:%3").arg((tmp / 60 / 60) % 60).arg((tmp / 60) % 60).arg(tmp % 60));
}

void PlayingWidget::positionChanged()
{
    player->setPosition(ui->durationSlider->sliderPosition()*3600);
}

void PlayingWidget::setVolume(float value)
{
    audioOutput->setVolume((float) value / 100);
    Settings::volume = value / 100; Settings s;
    s.saveSettings();
}

