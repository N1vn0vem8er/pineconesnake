#include "playlistswidget.h"
#include "playlistitemwidget.h"
#include "resourcesmanager.h"
#include "ui_playlistswidget.h"

PlaylistsWidget::PlaylistsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlaylistsWidget)
{
    ui->setupUi(this);
    ui->tableView->setVisible(false);
    loadPlaylists();
}

PlaylistsWidget::~PlaylistsWidget()
{
    delete ui;
}

void PlaylistsWidget::loadPlaylists()
{
    playlistNames = ResourcesManager::getInstance()->getAllPlaylistNames();
    if(layout != nullptr)
    {
        delete layout;
    }
    layout = new QGridLayout(ui->scrollAreaWidgetContents);
    if(!playlistWidgets.isEmpty())
        for(const auto& i : playlistWidgets) delete i;
    for(const auto& i : playlistNames)
    {
        PlaylistItemWidget* widget = new PlaylistItemWidget(i, this);
        playlistWidgets.append(widget);
        this->layout->addWidget(widget);
    }
    ui->scrollAreaWidgetContents->setLayout(layout);
}
