#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAudioOutput>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setVideoOutput(ui->videoWidget);
    player->setAudioOutput(audioOutput);

    ui->volumeSlider->setValue(audioOutput->volume());

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->playPauseButton, &QPushButton::clicked, this, [&]{if(player->isPlaying()) pause(); else play();});
    connect(ui->volumeSlider, &QSlider::sliderMoved, this, [&](int position){audioOutput->setVolume(static_cast<float>(position) / 100);});
    connect(ui->muteButton, &QPushButton::clicked, this, &MainWindow::mute);
    connect(ui->seekForwardButton, &QPushButton::clicked, this, &MainWindow::seekForward);
    connect(ui->seekBackwardButton, &QPushButton::clicked, this, &MainWindow::seekBackward);
    connect(player, &QMediaPlayer::durationChanged, ui->playingSlider, &QSlider::setMaximum);
    connect(ui->playingSlider, &QSlider::valueChanged, player, &QMediaPlayer::setPosition);
    connect(player, &QMediaPlayer::positionChanged, ui->playingSlider, &QSlider::setValue);
    connect(ui->actionPlayPause, &QAction::triggered, this, [&]{if(player->isPlaying()) pause(); else play();});
    connect(ui->actionSkip_forward, &QAction::triggered, this, &MainWindow::seekForward);
    connect(ui->actionSkip_backward, &QAction::triggered, this, &MainWindow::seekBackward);
    connect(ui->actionNext, &QAction::triggered, this, &MainWindow::next);
    connect(ui->actionPrevious, &QAction::triggered, this, &MainWindow::previous);
    connect(ui->actionIncrease_volume, &QAction::triggered, this, &MainWindow::increaseVolume);
    connect(ui->actionDecrease_volume, &QAction::triggered, this, &MainWindow::decreaseVolume);
    connect(audioOutput, &QAudioOutput::volumeChanged, this, [&](float volume){ui->volumeSlider->setValue(volume * 100);});
    connect(ui->actionMute, &QAction::triggered, this, &MainWindow::mute);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath());
    if(!path.isEmpty())
    {
        player->setSource(QUrl(path));
        play();
    }
}

void MainWindow::play()
{
    player->play();
    ui->playPauseButton->setIcon(mediaPauseIcon);
    ui->playPauseButton->setToolTip(tr("Pause"));
    ui->actionPlayPause->setIcon(mediaPauseIcon);
    ui->actionPlayPause->setText(tr("Pause"));
}

void MainWindow::pause()
{
    player->pause();
    ui->playPauseButton->setIcon(mediaPlayIcon);
    ui->playPauseButton->setToolTip(tr("Play"));
    ui->actionPlayPause->setIcon(mediaPlayIcon);
    ui->actionPlayPause->setText(tr("Play"));
}

void MainWindow::seekForward()
{
    if(player->isSeekable())
    {
        player->setPosition(player->position() + 5000);
    }
}

void MainWindow::seekBackward()
{
    if(player->isSeekable())
    {
        player->setPosition(player->position() - 5000);
    }
}

void MainWindow::next()
{

}

void MainWindow::previous()
{

}

void MainWindow::mute()
{
    if(audioOutput->isMuted())
    {
        audioOutput->setMuted(false);
        ui->muteButton->setToolTip(tr("Mute"));
        ui->muteButton->setIcon(volumeHighIcon);
        ui->actionMute->setText(tr("Mute"));
        ui->actionMute->setIcon(volumeHighIcon);
    }
    else
    {
        audioOutput->setMuted(true);
        ui->muteButton->setToolTip(tr("Un mute"));
        ui->muteButton->setIcon(volumeMuteIcon);
        ui->actionMute->setText(tr("Un mute"));
        ui->actionMute->setIcon(volumeMuteIcon);
    }
}

void MainWindow::increaseVolume()
{
    audioOutput->setVolume(audioOutput->volume() + 0.1);
}

void MainWindow::decreaseVolume()
{
    audioOutput->setVolume(audioOutput->volume() - 0.1);
}
