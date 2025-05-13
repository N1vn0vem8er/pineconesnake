#include "playlistswidget.h"
#include "playlistitemwidget.h"
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
        for(const auto& i : playlistWidgets) delete i;
        playlistWidgets.clear();
    }
    for(const auto& i : playlistNames)
    {
        PlaylistItemWidget* widget = new PlaylistItemWidget(i, this);
        connect(widget, &PlaylistItemWidget::doubleClicked, this, &PlaylistsWidget::playlistSelected);
        connect(widget, &PlaylistItemWidget::play, this, &PlaylistsWidget::playPlaylist);
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

}

void PlaylistsWidget::addToPlaylistPressed(int index)
{

}

void PlaylistsWidget::playPlaylist(const QString &name)
{
    Playlist pl = ResourcesManager::getInstance()->getPlaylistByName(name);
    if(pl.id != -1)
        playlistPlay(pl);
}
