#include "tilewidget.h"
#include "src/ui_tilewidget.h"

TileWidget::TileWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TileWidget)
{
    ui->setupUi(this);
}

TileWidget::~TileWidget()
{
    delete ui;
}

void TileWidget::setText(const QString &text)
{
    ui->label->setText(text);
}
