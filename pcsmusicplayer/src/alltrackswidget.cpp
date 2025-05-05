#include "alltrackswidget.h"
#include "resourcesmanager.h"
#include "ui_alltrackswidget.h"

AllTracksWidget::AllTracksWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AllTracksWidget)
{
    ui->setupUi(this);
    ui->searchLine->setVisible(false);
    delegate = new TrackItemWidget(ui->tableView);
    ui->tableView->setItemDelegate(delegate);
    connect(delegate, &TrackItemWidget::addToPlaylist, this, qOverload<int>(&AllTracksWidget::addToPlayList));
    connect(delegate, &TrackItemWidget::playPressed, this, &AllTracksWidget::playPressed);
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &AllTracksWidget::sort);
    loadTracks();
}

AllTracksWidget::~AllTracksWidget()
{
    delete ui;
}

void AllTracksWidget::loadTracks()
{
    tracks = ResourcesManager::getInstance()->getAllTracks();
    displayTracks();
}

void AllTracksWidget::displayTracks()
{
    if(model != nullptr)
    {
        delete model;
    }
    model = new TrackListModel(this);
    model->setItems(tracks);
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(3, 32);
    ui->tableView->setColumnWidth(4, 32);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void AllTracksWidget::addToPlayList(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        emit addToPlaylist(tracks[index]);
    }
}

void AllTracksWidget::playPressed(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        emit play(tracks[index]);
    }
}

void AllTracksWidget::sort(int index)
{
    switch (index) {
    case 0:
        std::sort(tracks.begin(), tracks.end(), [](const Track& track1, const Track& track2){return track1.album < track2.album;});
        break;
    case 1:
        std::sort(tracks.begin(), tracks.end(), [](const Track& track1, const Track& track2){return track1.title < track2.title;});
        break;
    case 2:
        std::sort(tracks.begin(), tracks.end(), [](const Track& track1, const Track& track2){return track1.artist < track2.artist;});
        break;
    case 3:
        std::sort(tracks.begin(), tracks.end(), [](const Track& track1, const Track& track2){return track1.played < track2.played;});
        break;
    default:
        break;
    }
    displayTracks();
}
