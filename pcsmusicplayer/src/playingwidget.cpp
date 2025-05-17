#include "playingwidget.h"
#include "qdialog.h"
#include "qfileinfo.h"
#include "resourcesmanager.h"
#include "settings.h"
#include "settingswidget.h"
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
    connect(ui->pauseButton, &QPushButton::clicked, this, [&]{player->pause(); ui->playButton->setVisible(true); ui->pauseButton->setVisible(false);ui->playButton->setFocus();});
    connect(ui->playButton, &QPushButton::clicked, this, [&]{player->play(); ui->playButton->setVisible(false); ui->pauseButton->setVisible(true);ui->pauseButton->setFocus();});
    connect(player, &QMediaPlayer::positionChanged, this, &PlayingWidget::diplayPostion);
    connect(ui->volume, &QSlider::valueChanged, this, &PlayingWidget::setVolume);
    connect(ui->durationSlider, &QSlider::sliderReleased, this, &PlayingWidget::positionChanged);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [&](QMediaPlayer::MediaStatus status){if(status == QMediaPlayer::MediaStatus::EndOfMedia) emit trackFinished(track);});
    connect(ui->nextButton, &QPushButton::clicked, this, [&]{emit playNext();});
    connect(ui->previousButton, &QPushButton::clicked, this, [&]{emit playPreviout();});
    connect(ui->settingsButton, &QPushButton::clicked, this, &PlayingWidget::openSettings);
    Settings s;
    s.loadSettings();
    setVolume(Settings::volume * 100);
    ui->volume->setValue(Settings::volume * 100);
    ui->playButton->setVisible(false);
    ui->pauseButton->setVisible(false);
    ui->nextButton->setVisible(false);
    ui->previousButton->setVisible(false);
}

PlayingWidget::~PlayingWidget()
{
    delete audioOutput;
    delete ui;
}

void PlayingWidget::play(const Track &track)
{
    this->track = track;
    this->track.played ++;
    player->setAudioOutput(audioOutput);
    ui->titleLabel->setText(this->track.title.isEmpty() ? QFileInfo(this->track.path).fileName() : this->track.title);
    ui->durationLabel->setText(QString("%1:%2:%3").arg((this->track.length / 60 / 60) % 60).arg((this->track.length / 60) % 60).arg(this->track.length % 60));
    ui->durationSlider->setMaximum(this->track.length);
    player->setSource(QUrl(this->track.path));
    player->play();
    ResourcesManager::getInstance()->modifyTrack(this->track);
    emit playingTrack(this->track);
    ui->pauseButton->setVisible(true);
    ui->pauseButton->setFocus();
    ui->nextButton->setVisible(true);
    ui->previousButton->setVisible(true);
}

void PlayingWidget::diplayPostion(qint64 position)
{
    qint64 tmp = position / 1000;
    ui->durationSlider->setValue(tmp);
    ui->timePassedLabel->setText(QString("%1:%2:%3").arg((tmp / 60 / 60) % 60).arg((tmp / 60) % 60).arg(tmp));
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

void PlayingWidget::openSettings()
{
    QDialog* dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    QLayout* layout = new QHBoxLayout(dialog);
    layout->addWidget(new SettingsWidget(dialog));
    dialog->setLayout(layout);
    dialog->show();
}

