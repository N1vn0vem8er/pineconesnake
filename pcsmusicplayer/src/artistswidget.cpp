#include "artistswidget.h"
#include "artistitemwidget.h"
#include "ui_artistswidget.h"
#include "resourcesmanager.h"

ArtistsWidget::ArtistsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ArtistsWidget)
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, &ArtistsWidget::backPressed);
    ui->tableView->setVisible(false);
    ui->backButton->setVisible(false);
    delegate = new TrackItemWidget(ui->tableView);
    ui->tableView->setItemDelegate(delegate);
    connect(delegate, &TrackItemWidget::addToPlaylist, this, &ArtistsWidget::addToPlaylistPressed);
    connect(delegate, &TrackItemWidget::playPressed, this, &ArtistsWidget::playPressed);
    connect(delegate, &TrackItemWidget::makeFavorite, this, &ArtistsWidget::madeFavorite);
    loadArtists();
}

ArtistsWidget::~ArtistsWidget()
{
    delete ui;
}

void ArtistsWidget::loadArtists()
{
    if(layout != nullptr)
    {
        delete layout;
    }
    layout = new QGridLayout(ui->scrollAreaWidgetContents);
    if(!artistWidgets.isEmpty())
        for(const auto& i : artistWidgets) delete i;
    artists = ResourcesManager::getInstance()->getAllArtists();
    for(const auto& i : artists)
    {
        ArtistItemWidget* widget = new ArtistItemWidget(i, this);
        connect(widget, &ArtistItemWidget::doubleClicked, this, &ArtistsWidget::openTracks);
        artistWidgets.append(widget);
        this->layout->addWidget(widget);
    }
    ui->scrollAreaWidgetContents->setLayout(layout);
}

void ArtistsWidget::backPressed()
{
    ui->backButton->setVisible(false);
    ui->tableView->setVisible(false);
    ui->scrollArea->setVisible(true);
}

void ArtistsWidget::addToPlaylistPressed(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        emit addToPlaylist(tracks.at(index));
    }
}

void ArtistsWidget::playPressed(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        emit play(tracks.at(index));
    }
}

void ArtistsWidget::openTracks(const QString &title)
{
    ui->tableView->setVisible(true);
    ui->backButton->setVisible(true);
    ui->scrollArea->setVisible(false);
    if(model != nullptr)
    {
        delete model;
    }
    model = new TrackListModel(this);
    tracks = ResourcesManager::getInstance()->getAllTracksForArtist(title);
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

void ArtistsWidget::madeFavorite(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        Track t = tracks.at(index);
        t.favorite = !t.favorite;
        emit makeFavorite(t);
    }
}
