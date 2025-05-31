#include "fullscreendialog.h"
#include "ui_fullscreendialog.h"

FullScreenDialog::FullScreenDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FullScreenDialog)
{
    ui->setupUi(this);
}

FullScreenDialog::~FullScreenDialog()
{
    delete ui;
}
