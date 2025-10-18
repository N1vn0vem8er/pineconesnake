#include "mainwindow.h"
#include "tilewidget.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::add);
    connect(ui->randomButton, &QPushButton::clicked, this, &MainWindow::selectRandom);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    ui->listWidget->clear();
    for(const QString& i : std::as_const(texts))
    {
        TileWidget* widget = new TileWidget(ui->listWidget);
        widget->setText(i);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(widget->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
    }
}

void MainWindow::open()
{

}

void MainWindow::save()
{

}

void MainWindow::saveAs()
{

}

void MainWindow::selectRandom()
{
    std::random_device rg;
    std::mt19937_64 generator(rg());
    std::uniform_int_distribution<int> random(0, texts.length() - 1);
    ui->selectedLabel->setText(texts.value(random(generator)));
}

void MainWindow::add()
{
    bool ok;
    const QString text = QInputDialog::getText(this, tr("Add"), tr(""), QLineEdit::Normal, "", &ok);
    if(ok)
    {
        texts.append(text);
        refresh();
    }
}

void MainWindow::remove()
{

}
