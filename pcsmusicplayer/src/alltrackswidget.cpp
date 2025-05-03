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
}

AllTracksWidget::~AllTracksWidget()
{
    delete ui;
}
