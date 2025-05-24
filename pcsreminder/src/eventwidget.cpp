#include "eventwidget.h"
#include "ui_eventwidget.h"

EventWidget::EventWidget(EventManager::Event &event, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EventWidget)
{
    ui->setupUi(this);
    this->event = event;
    refresh();
}

EventWidget::~EventWidget()
{
    delete ui;
}

void EventWidget::refresh()
{
    ui->titleLabel->setText(event.title);
    ui->contentLabel->setText(event.content);
    ui->enabledLabel->setText(event.enabled ? tr("Enabled") : tr("Disabled"));
    ui->dateLabel->setText(event.date);
    ui->enableButton->setVisible(event.enabled);
    ui->disableButton->setVisible(!event.enabled);
}
