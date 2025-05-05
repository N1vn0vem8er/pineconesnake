#include "mainwindow.h"
#include "resourcesmanager.h"
#include "searchworker.h"
#include "settings.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 1);
    startMusicSearch();
    connect(ui->songsTab, &AllTracksWidget::addToPlaylist, ui->playlistWidget, &PlaylistWidget::addTrack);
    connect(ui->songsTab, &AllTracksWidget::play, ui->playingWidget, &PlayingWidget::play);
    connect(ui->playingWidget, &PlayingWidget::trackFinished, ui->playlistWidget, &PlaylistWidget::trackFinished);
    connect(ui->playlistWidget, qOverload<const Track&>(&PlaylistWidget::playTrack), ui->playingWidget, &PlayingWidget::play);
    connect(ui->playlistWidget, qOverload<const Track&>(&PlaylistWidget::playTrack), ui->nowPlayingTab, &NowPlayingWidget::setTrack);
    connect(ui->songsTab, &AllTracksWidget::play, ui->nowPlayingTab, &NowPlayingWidget::setTrack);
    connect(ui->nowPlayingTab, &NowPlayingWidget::favorite, ui->songsTab, &AllTracksWidget::loadTracks);
    Settings s;
    s.loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startMusicSearch()
{
    if(searchThread != nullptr)
    {
        searchThread->requestInterruption();
        searchThread->wait();
    }
    searchThread = new QThread();
    SearchWorker* worker = new SearchWorker;
    worker->moveToThread(searchThread);
    connect(searchThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::startSearch, worker, &SearchWorker::doWork);
    connect(worker, &SearchWorker::resultReady, this, &MainWindow::searchResoultsReady);
    connect(worker, &SearchWorker::finished, searchThread, &QThread::quit);
    connect(searchThread, &QThread::finished, searchThread, &QThread::deleteLater);
    searchThread->start();
    emit startSearch(QDir::homePath() + "/Music", ResourcesManager::getInstance()->getAllPaths());
}

void MainWindow::searchResoultsReady(const QList<Track> tracks)
{
    ResourcesManager::getInstance()->saveTracks(tracks);
    ui->songsTab->loadTracks();
}
