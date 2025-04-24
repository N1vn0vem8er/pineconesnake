#include "writer.h"
#include "ui_writer.h"

Writer::Writer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Writer)
{
    ui->setupUi(this);
}

Writer::~Writer()
{
    delete ui;
}
