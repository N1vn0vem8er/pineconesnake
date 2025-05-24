#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    if(layout!=nullptr)
    {
        delete layout;
    }
    layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
}

void MainWindow::addReminderPressed()
{

}

void MainWindow::addRepeatingPressed()
{

}
