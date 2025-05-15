#include "fileswidget.h"
#include "resourcesmanager.h"
#include "ui_fileswidget.h"

FilesWidget::FilesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FilesWidget)
{
    ui->setupUi(this);
    delegate = new FilesListItemWidget(ui->tableView);
    ui->tableView->setItemDelegate(delegate);
    loadFiles();
}

FilesWidget::~FilesWidget()
{
    delete ui;
}

void FilesWidget::loadFiles()
{
    if(model != nullptr)
        delete model;
    model = new FilesListModel(this);
    tracks = ResourcesManager::getInstance()->getAllTracks();
    model->setItems(tracks);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->setColumnWidth(2, 32);
    ui->tableView->setColumnWidth(3, 32);
}

void FilesWidget::playPressed(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        emit play(tracks.at(index));
    }
}

void FilesWidget::addToPlaylistPressed(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        emit addToPlaylist(tracks.at(index));
    }
}

void FilesWidget::makeFavoritePressed(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        emit makeFavorite(tracks.at(index));
    }
}
