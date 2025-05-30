#include "eventwidget.h"
#include "editeventdialog.h"
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
    connect(ui->enableButton, &QPushButton::clicked, this, &EventWidget::enable);
    connect(ui->disableButton, &QPushButton::clicked, this, &EventWidget::disable);
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
    EditEventDialog* dialog = new EditEventDialog(event, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &EditEventDialog::editEvent, this, &EventWidget::editReminder);
    dialog->show();
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

void EventWidget::editReminder(const EventManager::Event &event)
{
    ResourcesManager::getInstance()->modifyEvent(event);
    refresh();
    emit requestRefresh();
}

void EventWidget::enable()
{
    event.enabled = true;
    ResourcesManager::getInstance()->modifyEvent(event);
    ui->enableButton->setVisible(false);
    ui->disableButton->setVisible(true);
    ui->disableButton->setFocus();
    emit requestRefresh();
}

void EventWidget::disable()
{
    event.enabled = false;
    ResourcesManager::getInstance()->modifyEvent(event);
    ui->enableButton->setVisible(true);
    ui->disableButton->setVisible(false);
    ui->enableButton->setFocus();
    emit requestRefresh();
}
