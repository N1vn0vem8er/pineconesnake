#include "mainwindow.h"
#include "tilewidget.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QInputDialog>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::add);
    connect(ui->randomButton, &QPushButton::clicked, this, &MainWindow::selectRandom);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
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
        connect(widget, &TileWidget::edited, this, &MainWindow::edited);
        widget->setText(i);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(widget->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
    }
}

void MainWindow::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            const QString buffer = file.readAll();
            file.close();
            texts = buffer.split(",");
            openedFile = path;
            refresh();
        }
    }
}

void MainWindow::save()
{
    if(!openedFile.isEmpty())
    {
        QFile file(openedFile);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(texts.join(",").toUtf8());
            file.close();
        }
    }
    else
        saveAs();
}

void MainWindow::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(texts.join(",").toUtf8());
            file.close();
            openedFile = path;
        }
    }
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

void MainWindow::edited(const QString &oldText, const QString &newText)
{
    const int index = texts.indexOf(oldText);
    texts.removeOne(oldText);
    texts.insert(index, newText);
    refresh();
}
