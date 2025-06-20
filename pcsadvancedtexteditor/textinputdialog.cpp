#include "textinputdialog.h"
#include "ui_textinputdialog.h"

TextInputDialog::TextInputDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TextInputDialog)
{
    ui->setupUi(this);
}

TextInputDialog::~TextInputDialog()
{
    delete ui;
}

QString TextInputDialog::getText() const
{

}
