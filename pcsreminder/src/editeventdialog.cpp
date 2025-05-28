#include "editeventdialog.h"
#include "ui_editeventdialog.h"

EditEventDialog::EditEventDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditEventDialog)
{
    ui->setupUi(this);
}

EditEventDialog::~EditEventDialog()
{
    delete ui;
}
