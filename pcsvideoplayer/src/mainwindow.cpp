#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAudioOutput>
#include <QFileDialog>
#include <QInputDialog>
#include <QTimer>
#include <QMessageBox>

#define VERSION "0.1.0"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"
#define PROJECTLINK "https://github.com/N1vn0vem8er/pineconesnake"

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
    oldWindowState = windowState();

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->playPauseButton, &QPushButton::clicked, this, [&]{if(player->isPlaying()) pause(); else play();});
    connect(ui->volumeSlider, &QSlider::sliderMoved, this, [&](int position){audioOutput->setVolume(static_cast<float>(position) / 100);});
    connect(ui->muteButton, &QPushButton::clicked, this, &MainWindow::mute);
    connect(ui->seekForwardButton, &QPushButton::clicked, this, &MainWindow::seekForward);
    connect(ui->seekBackwardButton, &QPushButton::clicked, this, &MainWindow::seekBackward);
    connect(player, &QMediaPlayer::durationChanged, ui->playingSlider, &QSlider::setMaximum);
    connect(ui->playingSlider, &QSlider::valueChanged, player, &QMediaPlayer::setPosition);
    connect(ui->actionPlayPause, &QAction::triggered, this, [&]{if(player->isPlaying()) pause(); else play();});
    connect(ui->actionSkip_forward, &QAction::triggered, this, &MainWindow::seekForward);
    connect(ui->actionSkip_backward, &QAction::triggered, this, &MainWindow::seekBackward);
    connect(ui->actionNext, &QAction::triggered, this, &MainWindow::next);
    connect(ui->nextButton, &QPushButton::clicked, this, &MainWindow::next);
    connect(ui->previousButton, &QPushButton::clicked, this, &MainWindow::previous);
    connect(ui->actionPrevious, &QAction::triggered, this, &MainWindow::previous);
    connect(ui->actionIncrease_volume, &QAction::triggered, this, &MainWindow::increaseVolume);
    connect(ui->actionDecrease_volume, &QAction::triggered, this, &MainWindow::decreaseVolume);
    connect(audioOutput, &QAudioOutput::volumeChanged, this, [&](float volume){ui->volumeSlider->setValue(volume * 100);});
    connect(ui->actionMute, &QAction::triggered, this, &MainWindow::mute);
    connect(ui->actionHide_bottom_panel, &QAction::triggered, this, &MainWindow::hideShowBottomPanel);
    connect(ui->actionFull_screen, &QAction::triggered, this, &MainWindow::fullScreen);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows);
    connect(ui->actionOpen_Url, &QAction::triggered, this, &MainWindow::openUrl);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [this]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_PCS_Video_Player, &QAction::triggered, this, &MainWindow::openAbout);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{ui->playingSlider->setValue(player->position());});
    timer->setInterval(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QFileInfoList MainWindow::getVideosInDir(const QString &path) const
{
    return QDir(path).entryInfoList({"*.mp4", "*.mkv"}, QDir::Files);
}

void MainWindow::playVideo(const QString& path)
{
    player->setSource(QUrl(path));
    timer->stop();
    timer->start();
    currentFile = path;
    play();
}

void MainWindow::openFile()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath());
    if(!path.isEmpty())
    {
        playVideo(path);
    }
}

void MainWindow::openUrl()
{
    const QString url = QInputDialog::getText(this, tr(""), tr("Input url"));
    if(!url.isEmpty())
    {
        playVideo(url);
        currentFile.clear();
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
    if(currentFile.isEmpty())
        return;
    const auto files = getVideosInDir(QFileInfo(currentFile).absoluteDir().absolutePath());
    int index = files.indexOf(QFileInfo(currentFile));
    if(index < 0)
        return;
    if(index >= files.size() || index + 1 >= files.size())
    {
        playVideo(files[0].absoluteFilePath());
    }
    else
    {
        playVideo(files[index + 1].absoluteFilePath());
    }
}

void MainWindow::previous()
{
    if(currentFile.isEmpty())
        return;
    const auto files = getVideosInDir(QFileInfo(currentFile).absoluteDir().absolutePath());
    int index = files.indexOf(QFileInfo(currentFile));
    if(index < 0)
        return;
    if(index == 0)
    {
        playVideo(files[files.length()-1].absoluteFilePath());
    }
    else
    {
        playVideo(files[index-1].absoluteFilePath());
    }
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

void MainWindow::hideShowBottomPanel()
{
    if(isBottomPanelVisible)
    {
        for(int i = 0; i < ui->bottomLayout->count(); i++)
        {
            ui->bottomLayout->itemAt(i)->widget()->setVisible(false);
        }
        ui->actionHide_bottom_panel->setText(tr("Show bottom panel"));
        isBottomPanelVisible = false;
    }
    else
    {
        for(int i = 0; i < ui->bottomLayout->count(); i++)
        {
            ui->bottomLayout->itemAt(i)->widget()->setVisible(true);
        }
        ui->actionHide_bottom_panel->setText(tr("Hide bottom panel"));
        isBottomPanelVisible = true;
    }
}

void MainWindow::fullScreen()
{
    if(windowState().testFlag(Qt::WindowFullScreen))
    {
        setWindowState(oldWindowState);
    }
    else
    {
        oldWindowState = windowState();
        setWindowState(Qt::WindowFullScreen);
    }
}

void MainWindow::openAbout()
{
    QMessageBox::about(this, tr("About Application"), tr(R"(
    <html>
        <body>
            <h2>Video Player</h2>
            <p>PCS Video Player is a simple video player. It is a part of the <a href="%3">Pinecone Snake project</a>.</p>
            <p>Version: %1</p>
            <p>License: <a href="%2">GPL 3</a></p>
        </body>
    </html>
    )").arg(VERSION, LICENSELINK, PROJECTLINK));
}
