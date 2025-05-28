#include "eventwidget.h"
#include "ui_eventwidget.h"

#include <QMessageBox>
#include <resourcesmanager.h>

EventWidget::EventWidget(const EventManager::Event &event, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EventWidget)
{
    ui->setupUi(this);
    connect(ui->deleteButton, &QPushButton::clicked, this, &EventWidget::deletePressed);
    connect(ui->editButton, &QPushButton::clicked, this, &EventWidget::editPressed);
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
    ui->enableButton->setVisible(!event.enabled);
    ui->disableButton->setVisible(event.enabled);
}

void EventWidget::editPressed()
{

}

void EventWidget::deletePressed()
{
    QMessageBox::StandardButton dialog = QMessageBox::question(this, tr("Are you sure?"), tr("Delete %1?").arg(event.title), QMessageBox::Yes | QMessageBox::No);
    if(dialog == QMessageBox::Yes)
    {
        ResourcesManager::getInstance()->deleteEvent(event);
        emit requestRefresh();
    }
}
