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
        for(const auto& i : albumWidgets) delete i;
    albums = ResourcesManager::getInstance()->getAllAlbums();
    for(const auto& i : albums)
    {
        AlbumItemWidget* widget = new AlbumItemWidget(i, this);
        connect(widget, &AlbumItemWidget::doubleClicked, this, &AlbumsWidget::openAlbum);
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
}

void AlbumsWidget::backPressed()
{
    ui->backButton->setVisible(false);
    ui->tableView->setVisible(false);
    ui->scrollArea->setVisible(true);
}
