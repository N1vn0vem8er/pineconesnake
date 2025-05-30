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
    if(seconds < 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Time must be greater then one second"));
        return;
    }
    switch(ui->comboBox->currentIndex())
    {
    case 0:
        emit editRepeating(EventManager::RepeatedEvent(repeated.id, title, content, seconds, repeated.enabled));
        break;
    case 1:
        emit editEvent(EventManager::Event(event.id, title, content, date, event.enabled));
        break;
    }
}
