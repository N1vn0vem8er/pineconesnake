#include "editeventdialog.h"
#include "ui_editeventdialog.h"

#include <QMessageBox>

EditEventDialog::EditEventDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditEventDialog)
{
    ui->setupUi(this);
    connect(this, &QDialog::accepted, this, &EditEventDialog::submit);
}

EditEventDialog::~EditEventDialog()
{
    delete ui;
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
}
