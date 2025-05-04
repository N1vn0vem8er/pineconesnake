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
    loadTracks();
}

AllTracksWidget::~AllTracksWidget()
{
    delete ui;
}

void AllTracksWidget::loadTracks()
{
    if(model != nullptr)
    {
        delete model;
    }
    model = new TrackListModel(this);
    tracks = ResourcesManager::getInstance()->getAllTracks();
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
