#include "favoritewidget.h"
#include "resourcesmanager.h"
#include "ui_favoritewidget.h"

FavoriteWidget::FavoriteWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FavoriteWidget)
{
    ui->setupUi(this);
    delegate = new FavoriteItemWidget(ui->tableView);
    ui->tableView->setItemDelegate(delegate);
    connect(delegate, &FavoriteItemWidget::playPressed, this, &FavoriteWidget::playPressed);
    connect(delegate, &FavoriteItemWidget::addToPlaylist, this, &FavoriteWidget::addToPlaylistPressed);
    connect(delegate, &FavoriteItemWidget::unFavorite, this, &FavoriteWidget::madeFavorite);
    loadFavorites();
}

FavoriteWidget::~FavoriteWidget()
{
    delete ui;
}

void FavoriteWidget::loadFavorites()
{
    if(model != nullptr)
    {
        delete model;
    }
    model = new FavoriteModel(ui->tableView);
    tracks = ResourcesManager::getInstance()->getAllFavoriteTracks();
    model->setItems(tracks);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->setColumnWidth(3, 32);
    ui->tableView->setColumnWidth(4, 32);
    ui->tableView->setColumnWidth(5, 32);
}

void FavoriteWidget::madeFavorite(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        Track t = tracks.at(index);
        t.favorite = !t.favorite;
        emit makeFavorite(t);
    }
}

void FavoriteWidget::playPressed(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        emit play(tracks.at(index));
    }
}

void FavoriteWidget::addToPlaylistPressed(int index)
{
    if(index >= 0 && index < tracks.length())
    {
        emit addToPlaylist(tracks.at(index));
    }
}
