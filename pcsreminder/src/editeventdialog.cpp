#include "editeventdialog.h"
#include "ui_editeventdialog.h"

#include <QMessageBox>

EditEventDialog::EditEventDialog(const EventManager::Event& event, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditEventDialog)
{
    ui->setupUi(this);
    this->event = event;
    ui->titleLine->setText(event.title);
    ui->descriptionLine->setText(event.content);
    ui->dateTimeEdit->setDateTime(QDateTime::fromString(event.date, "yyyy-MM-dd HH:mm:ss"));
    ui->showAsComboBox->setCurrentIndex(event.type);
    type = 1;
    init();
}

EditEventDialog::EditEventDialog(const EventManager::RepeatedEvent &event, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditEventDialog)
{
    ui->setupUi(this);
    this->repeated = event;
    ui->titleLine->setText(event.title);
    ui->descriptionLine->setText(event.title);
    ui->hoursLine->setText(QString::number(event.everySeconds / 3600));
    ui->minutesLine->setText(QString::number(event.everySeconds / 60));
    ui->secondsLine->setText(QString::number(event.everySeconds));
    ui->showAsComboBox->setCurrentIndex(event.type);
    type = 0;
    init();
}

EditEventDialog::~EditEventDialog()
{
    delete ui;
}

void EditEventDialog::init()
{
    connect(this, &QDialog::accepted, this, &EditEventDialog::submit);
    optionChanged(type);
}

void EditEventDialog::submit()
{
    const QString title = ui->titleLine->text();
    const QString content = ui->descriptionLine->text();
    const QString date = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    const int seconds = ui->hoursLine->text().toInt() * 3600 + ui->minutesLine->text().toInt() * 60 + ui->secondsLine->text().toInt();
    if(title.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Title connot be empty"));
        return;
    }
    switch(type)
    {
    case 0:
        if(seconds < 1)
        {
            QMessageBox::critical(this, tr("Error"), tr("Time must be greater then one second"));
            return;
        }
        emit editRepeating(EventManager::RepeatedEvent(repeated.id, title, content, seconds, repeated.enabled, ui->showAsComboBox->currentIndex()));
        break;
    case 1:
        emit editEvent(EventManager::Event(event.id, title, content, date, event.enabled, ui->showAsComboBox->currentIndex()));
        break;
    }
}

void EditEventDialog::optionChanged(int index)
{
    switch(index)
    {
    case 0:
        ui->hoursLabel->setVisible(true);
        ui->minutesLabel->setVisible(true);
        ui->secondsLabel->setVisible(true);
        ui->repeatEveryLabel->setVisible(true);
        ui->hoursLine->setVisible(true);
        ui->minutesLine->setVisible(true);
        ui->secondsLine->setVisible(true);
        ui->dateTimeLabel->setVisible(false);
        ui->dateTimeEdit->setVisible(false);
        break;
    case 1:
        ui->hoursLabel->setVisible(false);
        ui->minutesLabel->setVisible(false);
        ui->secondsLabel->setVisible(false);
        ui->repeatEveryLabel->setVisible(false);
        ui->hoursLine->setVisible(false);
        ui->minutesLine->setVisible(false);
        ui->secondsLine->setVisible(false);
        ui->dateTimeLabel->setVisible(true);
        ui->dateTimeEdit->setVisible(true);
        break;
    }
}
