#include "savewarningdialog.h"
#include "ui_savewarningdialog.h"

SaveWarningDialog::SaveWarningDialog(QString name, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SaveWarningDialog)
{
    ui->setupUi(this);
    ui->fileNameLabel->setText(name);
}

SaveWarningDialog::~SaveWarningDialog()
{
    delete ui;
}
