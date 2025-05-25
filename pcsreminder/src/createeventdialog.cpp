#include "createeventdialog.h"
#include "ui_createeventdialog.h"

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
    switch(ui->comboBox->currentIndex())
    {
    case 0:
    {
        EventManager::RepeatedEvent repeatedEvent(-1, ui->titleLine->text(), ui->descriptionLine->text(), (ui->hoursLine->text().toInt() * 3600 + ui->minutesLine->text().toInt() * 60 + ui->secondsLine->text().toInt() > 0? ui->secondsLine->text().toInt() : 1), true);
        emit addRepeating(repeatedEvent);
    }
        break;
    case 1:
    {
        EventManager::Event event(-1, ui->titleLine->text(), ui->descriptionLine->text(), ui->checkBox->isChecked(), ui->dateTimeEdit->dateTime().toString(""), true);
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
        ui->repeatLabel->setVisible(false);
        ui->checkBox->setVisible(false);
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
        ui->repeatLabel->setVisible(true);
        ui->checkBox->setVisible(true);
        break;
    }
}
