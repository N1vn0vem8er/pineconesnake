#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPdfDocument>
#include <QPdfView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);

    ui->splitter->setStretchFactor(1, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeTab(int index)
{
    QWidget* widget = ui->tabWidget->widget(index);
    if(widget)
    {
        ui->tabWidget->removeTab(index);
        QPdfView* pdfView = qobject_cast<QPdfView*>(widget);
        if(pdfView)
        {
            pdfView->document()->close();
        }
        widget->deleteLater();
    }
}

void MainWindow::open()
{
    const QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open"), QDir::homePath(), "*.pdf");
    if(!paths.isEmpty())
    {
        for(const QString& path : paths)
        {
            QPdfView* pdfView = new QPdfView(ui->tabWidget);
            QPdfDocument* document = new QPdfDocument(pdfView);
            document->load(path);
            pdfView->setDocument(document);
            ui->tabWidget->addTab(pdfView, path);
        }
    }
}
