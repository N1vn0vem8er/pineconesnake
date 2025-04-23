#include "allnoteswidget.h"
#include "ui_allnoteswidget.h"

AllNotesWidget::AllNotesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AllNotesWidget)
{
    ui->setupUi(this);
}

AllNotesWidget::~AllNotesWidget()
{
    delete ui;
}
