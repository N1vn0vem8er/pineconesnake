#include "repeatedeventwidget.h"
#include "ui_repeatedeventwidget.h"

RepeatedEventWidget::RepeatedEventWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RepeatedEventWidget)
{
    ui->setupUi(this);
}

RepeatedEventWidget::~RepeatedEventWidget()
{
    delete ui;
}
