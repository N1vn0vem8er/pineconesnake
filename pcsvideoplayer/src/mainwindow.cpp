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
    ui->playPauseButton->setIcon(QIcon::fromTheme("media-playback-pause"));
    ui->playPauseButton->setToolTip(tr("Pause"));
}

void MainWindow::pause()
{
    player->pause();
    ui->playPauseButton->setIcon(QIcon::fromTheme("media-playback-start"));
    ui->playPauseButton->setToolTip(tr("Play"));
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
        ui->muteButton->setIcon(QIcon::fromTheme("audio-volume-high"));
    }
    else
    {
        audioOutput->setMuted(true);
        ui->muteButton->setToolTip(tr("Un mute"));
        ui->muteButton->setIcon(QIcon::fromTheme("audio-volume-muted"));
    }
}
