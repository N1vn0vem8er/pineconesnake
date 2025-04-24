#include "mainwindow.h"
#include "inputtitledialog.h"
#include "ui_mainwindow.h"
#include "writer.h"
#include "allnoteswidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->newButton, &QPushButton::clicked, this, &MainWindow::addNewNote);
    connect(ui->allButton, &QPushButton::clicked, this, &MainWindow::showAllNotes);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, [&](int index){delete ui->tabWidget->widget(index);});
    addTab("Notes", new AllNotesWidget(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTab(const QString &title, QWidget *widget)
{
    ui->tabWidget->addTab(widget, title);
}

void MainWindow::addNewNote()
{
    InputTitleDialog dialog;
    if(dialog.exec() == QDialog::Accepted)
    {
        const QString title = dialog.getTitle();
        addTab(title, new Writer(this));
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    }
}

void MainWindow::showAllNotes()
{

}
