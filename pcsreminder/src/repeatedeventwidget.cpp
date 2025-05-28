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
    EditEventDialog* dialog = new EditEventDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
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
