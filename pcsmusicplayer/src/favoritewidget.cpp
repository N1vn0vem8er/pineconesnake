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
    model->setItems(ResourcesManager::getInstance()->getAllFavoriteTracks());
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->setColumnWidth(3, 32);
    ui->tableView->setColumnWidth(4, 32);
    ui->tableView->setColumnWidth(5, 32);
}
