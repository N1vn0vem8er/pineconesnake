#include "inputtitledialog.h"
#include "ui_inputtitledialog.h"

InputTitleDialog::InputTitleDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InputTitleDialog)
{
    ui->setupUi(this);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &QDialog::accept);
    setWindowFlags(Qt::FramelessWindowHint);
}

InputTitleDialog::~InputTitleDialog()
{
    delete ui;
}

QString InputTitleDialog::getTitle() const
{
    return ui->lineEdit->text();
}
