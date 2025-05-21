#include "playlistwidget.h"
#include "playlistitem.h"
#include "playlistmodel.h"
#include "playlistnamedialog.h"
#include "resourcesmanager.h"
#include "ui_playlistwidget.h"
#include <QFileInfo>
#include <QMessageBox>


PlaylistWidget::PlaylistWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlaylistWidget)
{
    init();
}

PlaylistWidget::PlaylistWidget(QList<Track> tracks, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlaylistWidget)
{
    init();
    loadTracks(tracks);
}

void PlaylistWidget::init()
{
    ui->setupUi(this);
    delegate = new PlaylistItem(ui->tableView);
    connect(delegate, &PlaylistItem::playPressed, this, qOverload<int>(&PlaylistWidget::playTrack));
    connect(delegate, &PlaylistItem::removePressed, this, QOverload<int>::of(&PlaylistWidget::removeTrack));
    connect(ui->playAllButton, &QPushButton::clicked, this, [&]{playTrack(0);});
    connect(ui->clearButton, &QPushButton::clicked, this, &PlaylistWidget::clear);
    connect(ui->saveButton, &QPushButton::clicked, this, &PlaylistWidget::savePlaylist);
    ui->tableView->setItemDelegate(delegate);
}

PlaylistWidget::~PlaylistWidget()
{
    delete ui;
}

void PlaylistWidget::loadTracks(const QList<Track>& tracks)
{
    if(model != nullptr)
    {
        delete model;
    }
    this->tracks = tracks;
    model = new PlaylistModel(ui->tableView);
    model->setItems(tracks);
    ui->tableView->setModel(model);
    if(!this->tracks.isEmpty())
    {
        ui->tableView->setColumnWidth(2, 32);
        ui->tableView->setColumnWidth(3, 32);
        ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    }
}

void PlaylistWidget::playingTrack(const Track &track)
{

}

void PlaylistWidget::addTrack(const Track &track)
{
    if(std::find_if(tracks.begin(), tracks.end(), ([&track](const Track& t){return t.path == track.path && t.title == track.title;})) == tracks.end())
    {
        tracks.append(track);
        loadTracks(tracks);
    }
}

void PlaylistWidget::trackFinished(const Track &track)
{
    if(std::find_if(tracks.begin(), tracks.end(), ([&track](const Track& t){return t.path == track.path && t.title == track.title;})) != tracks.end())
    {
        playTrack(playingIndex + 1);
    }
}

void PlaylistWidget::loadPlaylist(const Playlist &playlist)
{
    this->playlist = playlist;
    for(const auto& i : std::as_const(this->playlist.tracks))
    {
        addTrack(i);
    }
}

void PlaylistWidget::playNext()
{
    if(playingIndex + 1 >= tracks.length())
    {
        playTrack(0);
    }
    else
    {
        playTrack(playingIndex+1);
    }
}

void PlaylistWidget::playPrevious()
{
    if(playingIndex - 1 < 0)
    {
        playTrack(tracks.length()-1);
    }
    else
    {
        playTrack(playingIndex-1);
    }
}

void PlaylistWidget::removeTrack(const Track &track)
{
    tracks.removeIf([&track](const Track& t){return t.id == track.id;});
    loadTracks(tracks);
}

void PlaylistWidget::playTrack(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        ui->tableView->selectRow(index);
        playingIndex = index;
        emit playTrack(tracks[index]);
    }
}

void PlaylistWidget::removeTrack(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        tracks.removeAt(index);
        loadTracks(tracks);
    }
}

void PlaylistWidget::clear()
{
    playlist = Playlist(-1, "", {});
    loadTracks({});
}

void PlaylistWidget::savePlaylist()
{
    if(playlist.id != -1)
    {
        playlist.tracks = tracks;
        ResourcesManager::getInstance()->modifyPlaylist(playlist);
        return;
    }
    PlaylistNameDialog* dialog = new PlaylistNameDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &PlaylistNameDialog::playlistName, this, &PlaylistWidget::playlistName);
    dialog->show();
}

void PlaylistWidget::playlistName(const QString &name)
{
    try
    {
        ResourcesManager::getInstance()->savePlaylist(Playlist(-1, name, tracks));
        emit playlistSaved();
    }
    catch (std::exception&)
    {
        QMessageBox::critical(this, tr("Critical"), tr("Playlist with that name exists"));
        savePlaylist();
    }
}
