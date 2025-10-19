#include "tilewidget.h"
#include "src/ui_tilewidget.h"
#include <QInputDialog>

TileWidget::TileWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TileWidget)
{
    ui->setupUi(this);
    connect(ui->editButton, &QPushButton::clicked, this, &TileWidget::edit);
}

TileWidget::~TileWidget()
{
    delete ui;
}

void TileWidget::setText(const QString &text)
{
    ui->label->setText(text);
}

void TileWidget::edit()
{
    bool ok;
    const QString text = QInputDialog::getText(this, tr("Edit"), tr("New Text"), QLineEdit::Normal, "", &ok);
    if(ok)
    {
        emit edited(ui->label->text(), text);
        setText(text);
    }
}
