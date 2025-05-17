#include "albumswidget.h"
#include "albumitemwidget.h"
#include "resourcesmanager.h"
#include "ui_albumswidget.h"

#include <QHBoxLayout>

AlbumsWidget::AlbumsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AlbumsWidget)
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, &AlbumsWidget::backPressed);
    ui->tableView->setVisible(false);
    ui->backButton->setVisible(false);
    delegate = new TrackItemWidget(ui->tableView);
    ui->tableView->setItemDelegate(delegate);
    connect(delegate, &TrackItemWidget::addToPlaylist, this, &AlbumsWidget::addToPlaylistPressed);
    connect(delegate, &TrackItemWidget::playPressed, this, &AlbumsWidget::playPressed);
    loadAlbums();
}

AlbumsWidget::~AlbumsWidget()
{
    delete ui;
}

void AlbumsWidget::loadAlbums()
{
    if(layout != nullptr)
    {
        delete layout;
    }
    layout = new QGridLayout(ui->scrollAreaWidgetContents);
    if(!albumWidgets.isEmpty())
    {
        for(const auto& i : albumWidgets) delete i;
        albumWidgets.clear();
    }
    albums = ResourcesManager::getInstance()->getAllAlbums();
    for(const auto& i : albums)
    {
        AlbumItemWidget* widget = new AlbumItemWidget(i, this);
        connect(widget, &AlbumItemWidget::doubleClicked, this, &AlbumsWidget::openAlbum);
        connect(widget, &AlbumItemWidget::addToPlaylist, this, &AlbumsWidget::addAllTracksToPlaylist);
        albumWidgets.append(widget);
        this->layout->addWidget(widget);
    }
    ui->scrollAreaWidgetContents->setLayout(layout);
}

void AlbumsWidget::openAlbum(const QString &title)
{
    ui->tableView->setVisible(true);
    ui->backButton->setVisible(true);
    ui->scrollArea->setVisible(false);
    if(model != nullptr)
    {
        delete model;
    }
    model = new TrackListModel(this);
    tracks = ResourcesManager::getInstance()->getAllTrackInAlbum(title);
    std::sort(tracks.begin(), tracks.end(), [](const Track& track1, const Track& track2){return track1.number < track2.number;});
    model->setItems(tracks);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView->setColumnWidth(4, 32);
    ui->tableView->setColumnWidth(5, 32);
    ui->tableView->setColumnWidth(6, 32);
}

void AlbumsWidget::backPressed()
{
    ui->backButton->setVisible(false);
    ui->tableView->setVisible(false);
    ui->scrollArea->setVisible(true);
}

void AlbumsWidget::playPressed(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        emit play(tracks.at(index));
    }
}

void AlbumsWidget::addToPlaylistPressed(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        emit addToPlaylist(tracks.at(index));
    }
}

void AlbumsWidget::addAllTracksToPlaylist(QList<Track> tracks)
{
    std::sort(tracks.begin(), tracks.end(), [](const Track& a, const Track& b){return a.number < b.number;});
    for(const auto& i : tracks)
    {
        emit addToPlaylist(i);
    }
}
