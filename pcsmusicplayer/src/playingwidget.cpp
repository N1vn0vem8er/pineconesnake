#include "playingwidget.h"
#include "ui_playingwidget.h"

PlayingWidget::PlayingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayingWidget)
{
    ui->setupUi(this);
}

PlayingWidget::~PlayingWidget()
{
    delete ui;
}
