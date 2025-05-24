#include "eventwidget.h"
#include "ui_eventwidget.h"

EventWidget::EventWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EventWidget)
{
    ui->setupUi(this);
}

EventWidget::~EventWidget()
{
    delete ui;
}
