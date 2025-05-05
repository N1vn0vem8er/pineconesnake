#include "nowplayingwidget.h"
#include "qfileinfo.h"
#include "resourcesmanager.h"
#include "ui_nowplayingwidget.h"

NowPlayingWidget::NowPlayingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NowPlayingWidget)
{
    ui->setupUi(this);
    ui->pushButton->setVisible(false);
    connect(ui->pushButton, &QPushButton::clicked, this, &NowPlayingWidget::makeFavorite);
}

NowPlayingWidget::~NowPlayingWidget()
{
    delete ui;
}

void NowPlayingWidget::setTrack(const Track &track)
{
    ui->pushButton->setVisible(true);
    ui->titleLabel->setText(track.title.isEmpty() ? QFileInfo(track.path).fileName() : track.title);
    ui->albumLabel->setText(track.album);
    ui->artistLabel->setText(track.artist);
    ui->playedLabel->setText(QString::number(track.played));
    ui->favoriteLabel->setText(track.favorite ? tr("Favorite") : tr("Not favorite"));
    this->track = track;
}

void NowPlayingWidget::makeFavorite()
{
    track.favorite = !track.favorite;
    ResourcesManager::getInstance()->modifyTrack(track);
    setTrack(track);
    emit favorite();
}
