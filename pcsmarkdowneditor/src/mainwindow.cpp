#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("pcsmarkdowneditor");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
}

MainWindow::~MainWindow()
{
    QSettings settings("pcsmarkdowneditor");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    delete ui;
}
