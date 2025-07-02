#include "textinputdialog.h"
#include "ui_textinputdialog.h"

TextInputDialog::TextInputDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TextInputDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    resize(QSize(200, 100));
    QAction* close = new QAction(this);
    close->setShortcut(Qt::Key_Escape);
    connect(close, &QAction::triggered, this, &TextInputDialog::close);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &TextInputDialog::accept);
}

TextInputDialog::~TextInputDialog()
{
    delete ui;
}

QString TextInputDialog::getText() const
{
    return ui->lineEdit->text();
}
