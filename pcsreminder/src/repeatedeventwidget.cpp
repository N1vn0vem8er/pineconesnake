#include "repeatedeventwidget.h"
#include "ui_repeatedeventwidget.h"

RepeatedEventWidget::RepeatedEventWidget(const EventManager::RepeatedEvent &event, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RepeatedEventWidget)
{
    ui->setupUi(this);
    this->event = event;
    refresh();
}

RepeatedEventWidget::~RepeatedEventWidget()
{
    delete ui;
}

void RepeatedEventWidget::refresh()
{
    ui->titleLabel->setText(event.title);
    ui->contentLabel->setText(event.content);
    ui->enabledLabel->setText(event.enabled ? tr("Enabled") : tr("Disabled"));
    ui->repearEveryLabel->setText(QString::number(event.everySeconds));
    ui->enableButton->setVisible(event.enabled);
    ui->disableButton->setVisible(!event.enabled);
}
