#include "createeventdialog.h"
#include "ui_createeventdialog.h"

#include <QMessageBox>

CreateEventDialog::CreateEventDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateEventDialog)
{
    ui->setupUi(this);
    connect(this, &QDialog::accepted, this, &CreateEventDialog::okPressed);
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &CreateEventDialog::optionChanged);
    optionChanged(0);
}

CreateEventDialog::~CreateEventDialog()
{
    delete ui;
}

void CreateEventDialog::okPressed()
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
    switch(ui->comboBox->currentIndex())
    {
    case 0:
    {
        if(seconds < 1)
        {
            QMessageBox::critical(this, tr("Error"), tr("Time must be greater then one second"));
            return;
        }
        EventManager::RepeatedEvent repeatedEvent(-1, title, content, seconds, true, ui->showAsComboBox->currentIndex());
        emit addRepeating(repeatedEvent);
    }
        break;
    case 1:
    {
        EventManager::Event event(-1, title, content, date, true, ui->showAsComboBox->currentIndex());
        emit addEvent(event);
    }
        break;
    }
}

void CreateEventDialog::optionChanged(int index)
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
