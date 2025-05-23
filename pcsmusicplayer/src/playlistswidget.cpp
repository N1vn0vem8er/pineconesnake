#include "playlistswidget.h"
#include "playlistitemwidget.h"
#include "qmessagebox.h"
#include "resourcesmanager.h"
#include "ui_playlistswidget.h"

PlaylistsWidget::PlaylistsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlaylistsWidget)
{
    ui->setupUi(this);
    ui->tableView->setVisible(false);
    ui->pushButton->setVisible(false);
    connect(ui->pushButton, &QPushButton::clicked, this, &PlaylistsWidget::loadPlaylists);
    loadPlaylists();
    delegate = new PlaylistItem(ui->tableView);
    connect(delegate, &PlaylistItem::playPressed, this, &PlaylistsWidget::playPressed);
    connect(delegate, &PlaylistItem::removePressed, this, &PlaylistsWidget::removeFormPlaylist);
    ui->tableView->setItemDelegate(delegate);
}

PlaylistsWidget::~PlaylistsWidget()
{
    delete ui;
}

void PlaylistsWidget::loadPlaylists()
{
    ui->tableView->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->scrollArea->setVisible(true);
    playlistNames = ResourcesManager::getInstance()->getAllPlaylistNames();
    if(layout != nullptr)
    {
        delete layout;
    }
    layout = new QGridLayout(ui->scrollAreaWidgetContents);
    if(!playlistWidgets.isEmpty())
    {
        for(const auto& i : std::as_const(playlistWidgets)) delete i;
        playlistWidgets.clear();
    }
    for(const auto& i : std::as_const(playlistNames))
    {
        PlaylistItemWidget* widget = new PlaylistItemWidget(i, this);
        connect(widget, &PlaylistItemWidget::doubleClicked, this, &PlaylistsWidget::playlistSelected);
        connect(widget, &PlaylistItemWidget::play, this, &PlaylistsWidget::playPlaylist);
        connect(widget, &PlaylistItemWidget::del, this, &PlaylistsWidget::deletePlaylist);
        playlistWidgets.append(widget);
        this->layout->addWidget(widget);
    }
    ui->scrollAreaWidgetContents->setLayout(layout);
}

void PlaylistsWidget::playlistSelected(const QString &name)
{
    ui->tableView->setVisible(true);
    ui->pushButton->setVisible(true);
    ui->scrollArea->setVisible(false);
    playlist = ResourcesManager::getInstance()->getPlaylistByName(name);
    if(model != nullptr)
    {
        delete model;
    }
    model = new PlaylistModel(ui->tableView);
    model->setItems(playlist.tracks);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->setColumnWidth(3, 32);
    ui->tableView->setColumnWidth(4, 32);
}

void PlaylistsWidget::playPressed(int index)
{
    if(index >= 0 && index < playlist.tracks.size())
    {
        emit play(playlist.tracks.at(index));
    }
}

void PlaylistsWidget::addToPlaylistPressed(int index)
{

}

void PlaylistsWidget::playPlaylist(const QString &name)
{
    Playlist pl = ResourcesManager::getInstance()->getPlaylistByName(name);
    if(pl.id != -1)
        emit playlistPlay(pl);
}

void PlaylistsWidget::deletePlaylist(const QString& name)
{
    QMessageBox::StandardButton dialog = QMessageBox::question(this, tr("Are you sure?"), tr("Delete playlist?"), QMessageBox::Yes | QMessageBox::No);
    if(dialog == QMessageBox::Yes)
    {
        ResourcesManager* rm = ResourcesManager::getInstance();
        Playlist playlist = rm->getPlaylistByName(name);
        rm->deletePlaylist(playlist.id);
        loadPlaylists();
    }
}

void PlaylistsWidget::removeFormPlaylist(int index)
{
    if(index >= 0 && index < playlist.tracks.size())
    {
        Track t = playlist.tracks.at(index);
        ResourcesManager::getInstance()->removeFromPlaylist(playlist.id, t.id);
        playlistSelected(playlist.name);
    }
}
