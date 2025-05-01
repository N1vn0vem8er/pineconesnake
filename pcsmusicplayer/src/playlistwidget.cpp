#include "playlistwidget.h"
#include "playlistitem.h"
#include "playlistmodel.h"
#include "ui_playlistwidget.h"


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
    connect(delegate, &PlaylistItem::playPressed, this, &PlaylistWidget::playTrack);
    connect(delegate, &PlaylistItem::removePressed, this, QOverload<int>::of(&PlaylistWidget::removeTrack));
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
    QStringList titles;
    std::for_each(this->tracks.begin(), this->tracks.end(), [&](const Track& track){titles.append(track.title);});
    model = new PlaylistModel(titles, ui->tableView);
    ui->tableView->setModel(model);
    if(!this->tracks.isEmpty())
    {
        ui->tableView->setColumnWidth(1, 32);
        ui->tableView->setColumnWidth(2, 32);
        ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    }
}

void PlaylistWidget::playingTrack(const Track &track)
{

}

void PlaylistWidget::addTrack(const Track &track)
{

}

void PlaylistWidget::removeTrack(const Track &track)
{

}

void PlaylistWidget::playTrack(int index)
{
    qDebug() << index;
}

void PlaylistWidget::removeTrack(int index)
{
    qDebug() << index;
}
