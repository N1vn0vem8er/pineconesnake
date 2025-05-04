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
    model = new TrackListModel(this);
    model->setItems(ResourcesManager::getInstance()->getAllTracks());
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(3, 32);
    ui->tableView->setColumnWidth(4, 32);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

AllTracksWidget::~AllTracksWidget()
{
    delete ui;
}
