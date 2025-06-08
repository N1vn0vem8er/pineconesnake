#include "fullscreendialog.h"
#include "resourcesmanager.h"
#include "ui_fullscreendialog.h"

FullScreenDialog::FullScreenDialog(const EventManager::Event &event, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FullScreenDialog)
{
    ui->setupUi(this);
    this->event = event;
    type = 1;
    init();
}

FullScreenDialog::FullScreenDialog(const EventManager::RepeatedEvent &event, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FullScreenDialog)
{
    ui->setupUi(this);
    this->repeated = event;
    type = 0;
    init();
}

FullScreenDialog::~FullScreenDialog()
{
    delete ui;
}

void FullScreenDialog::init()
{
    connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->disableButton, &QPushButton::clicked, this, &FullScreenDialog::disable);
    switch(type)
    {
    case 0:
        ui->titleLabel->setText(repeated.title);
        ui->contentLabel->setText(repeated.content);
        ui->timeLabel->setText(tr("Every %1 seconds").arg(repeated.everySeconds));
        break;
    case 1:
        ui->titleLabel->setText(event.title);
        ui->contentLabel->setText(event.content);
        ui->timeLabel->setText(event.date);
        break;
    }
}

void FullScreenDialog::disable()
{
    switch(type)
    {
    case 0:
        repeated.enabled = false;
        ResourcesManager::getInstance()->modifyRepeating(repeated);
        break;
    case 1:
        event.enabled = false;
        ResourcesManager::getInstance()->modifyEvent(event);
        break;
    }
    emit requestRefresh();
}
