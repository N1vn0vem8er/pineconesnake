#include "mainwindow.h"
#include "playlistswidget.h"
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
    connect(ui->nowPlayingTab, &NowPlayingWidget::makeFavorite, this, &MainWindow::makeFavorite);
    connect(ui->favoritesTab, &FavoriteWidget::makeFavorite, this, &MainWindow::makeFavorite);
    connect(ui->favoritesTab, &FavoriteWidget::play, ui->playingWidget, &PlayingWidget::play);
    connect(ui->favoritesTab, &FavoriteWidget::addToPlaylist, ui->playlistWidget, &PlaylistWidget::addTrack);
    connect(ui->albumsTab, &AlbumsWidget::play, ui->playingWidget, &PlayingWidget::play);
    connect(ui->albumsTab, &AlbumsWidget::addToPlaylist, ui->playlistWidget, &PlaylistWidget::addTrack);
    connect(ui->artistsTab, &ArtistsWidget::play, ui->playingWidget, &PlayingWidget::play);
    connect(ui->artistsTab, &ArtistsWidget::addToPlaylist, ui->playlistWidget, &PlaylistWidget::addTrack);
    connect(ui->playlistWidget, &PlaylistWidget::playlistSaved, ui->playlistsTab, &PlaylistsWidget::loadPlaylists);
    connect(ui->playlistsTab, &PlaylistsWidget::playlistPlay, ui->playlistWidget, &PlaylistWidget::loadPlaylist);
    connect(ui->songsTab, &AllTracksWidget::makeFavorite, this, &MainWindow::makeFavorite);
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

void MainWindow::makeFavorite(const Track &track)
{
    ResourcesManager::getInstance()->modifyTrack(track);
    ui->favoritesTab->loadFavorites();
}
