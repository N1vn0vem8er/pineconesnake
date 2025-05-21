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
    connect(delegate, &FilesListItemWidget::playPressed, this, &FilesWidget::playPressed);
    connect(delegate, &FilesListItemWidget::addToPlaylist, this, &FilesWidget::addToPlaylistPressed);
    connect(ui->searchButton, &QPushButton::clicked, this, [&]{ui->searchBar->setVisible(!ui->searchBar->isVisible());});
    connect(ui->searchBar, &QLineEdit::textChanged, this, &FilesWidget::search);
        connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &FilesWidget::sort);
    ui->searchBar->setVisible(false);
    loadFiles();
}

FilesWidget::~FilesWidget()
{
    delete ui;
}

void FilesWidget::loadFiles()
{
    tracks = ResourcesManager::getInstance()->getAllTracks();
    displayFiles();
}

void FilesWidget::search()
{
    tracks = ResourcesManager::getInstance()->getAllTracks();
    QList<Track> tmp;
    const QString query = ui->searchBar->text();
    for(const auto& i : std::as_const(tracks))
    {
        if(i.title.contains(query) || i.album.contains(query) || i.artist.contains(query) || i.path.contains(query) || i.path.contains(query))
            tmp.append(i);
    }
    tracks = tmp;
    displayFiles();
}

void FilesWidget::displayFiles()
{
    if(model != nullptr)
        delete model;
    model = new FilesListModel(this);
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

void FilesWidget::sort(int index)
{
    switch (index) {
    case 0:
        std::sort(tracks.begin(), tracks.end(), [](const Track& track1, const Track& track2){return track1.title < track2.title;});
        break;
    case 1:
        std::sort(tracks.begin(), tracks.end(), [](const Track& track1, const Track& track2){return track1.played > track2.played;});
        break;
    case 2:
        std::sort(tracks.begin(), tracks.end(), [](const Track& track1, const Track& track2){return track1.album < track2.album;});
        break;
    case 3:
        std::sort(tracks.begin(), tracks.end(), [](const Track& track1, const Track& track2){return track1.artist < track2.artist;});
        break;
    case 4:
        std::sort(tracks.begin(), tracks.end(), [](const Track& track1, const Track& track2){return track1.path < track2.path;});
        break;
    default:
        break;
    }
    displayFiles();
}
