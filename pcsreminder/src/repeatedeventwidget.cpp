#include "repeatedeventwidget.h"
#include "editeventdialog.h"
#include "resourcesmanager.h"
#include "ui_repeatedeventwidget.h"

#include <QMessageBox>

RepeatedEventWidget::RepeatedEventWidget(const EventManager::RepeatedEvent &event, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RepeatedEventWidget)
{
    ui->setupUi(this);
    connect(ui->deleteButton, &QPushButton::clicked, this, &RepeatedEventWidget::deletePressed);
    connect(ui->editButton, &QPushButton::clicked, this, &RepeatedEventWidget::editPressed);
    connect(ui->enableButton, &QPushButton::clicked, this, &RepeatedEventWidget::enable);
    connect(ui->disableButton, &QPushButton::clicked, this, &RepeatedEventWidget::disable);
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
    ui->enableButton->setVisible(!event.enabled);
    ui->disableButton->setVisible(event.enabled);
}

void RepeatedEventWidget::editPressed()
{
    EditEventDialog* dialog = new EditEventDialog(event, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &EditEventDialog::editRepeating, this, &RepeatedEventWidget::editRepeating);
    dialog->show();
}

void RepeatedEventWidget::deletePressed()
{
    QMessageBox::StandardButton dialog = QMessageBox::question(this, tr("Are you sure?"), tr("Delete %1?").arg(event.title), QMessageBox::Yes | QMessageBox::No);
    if(dialog == QMessageBox::Yes)
    {
        ResourcesManager::getInstance()->deleteRepeating(event);
        emit requestRefresh();
    }
}

void RepeatedEventWidget::editRepeating(const EventManager::RepeatedEvent &event)
{
    ResourcesManager::getInstance()->modifyRepeating(event);
    refresh();
    emit requestRefresh();
}

void RepeatedEventWidget::enable()
{
    event.enabled = true;
    ResourcesManager::getInstance()->modifyRepeating(event);
    ui->enableButton->setVisible(false);
    ui->disableButton->setVisible(true);
    ui->disableButton->setFocus();
    emit requestRefresh();
}

void RepeatedEventWidget::disable()
{
    event.enabled = false;
    ResourcesManager::getInstance()->modifyRepeating(event);
    ui->enableButton->setVisible(true);
    ui->disableButton->setVisible(false);
    ui->enableButton->setFocus();
    emit requestRefresh();
}
