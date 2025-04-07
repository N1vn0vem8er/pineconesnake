#include "searchwidget.h"
#include "ui_searchwidget.h"

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SearchWidget)
{
    ui->setupUi(this);
    connect(ui->findButton, &QPushButton::clicked, this, QOverload<>::of(&SearchWidget::find));
    connect(ui->replaceButton, &QPushButton::clicked, this, QOverload<>::of(&SearchWidget::replace));
    connect(ui->exchangeButton, &QPushButton::clicked, this, &SearchWidget::exchange);
}

SearchWidget::~SearchWidget()
{
    delete ui;
}

void SearchWidget::find()
{
    if(!ui->searchLine->text().isEmpty())
    {
        emit find(ui->searchLine->text());
    }
}

void SearchWidget::replace()
{
    if(!ui->searchLine->text().isEmpty() && !ui->replaceLine->text().isEmpty())
    {
        emit replace(ui->searchLine->text(), ui->replaceLine->text());
    }
}

void SearchWidget::exchange()
{
    QString tmp = ui->searchLine->text();
    ui->searchLine->setText(ui->replaceLine->text());
    ui->replaceLine->setText(tmp);
}
