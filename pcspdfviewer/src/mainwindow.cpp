#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPdfBookmarkModel>
#include <QPdfDocument>
#include <QPdfView>
#include <qpdfpagenavigator.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionClose, &QAction::triggered, this, [&]{closeTab(ui->tabWidget->currentIndex());});
    connect(ui->actionClose_All, &QAction::triggered, this, [&]{while(ui->tabWidget->count() > 0)closeTab(ui->tabWidget->currentIndex());});
    connect(ui->actionClose_All_But_This, &QAction::triggered, this, [&]{for(int i=ui->tabWidget->count()-1; i >= 0;--i)if(i != ui->tabWidget->currentIndex())closeTab(i);});
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);
    connect(ui->treeView, &QAbstractItemView::activated, this, &MainWindow::bookmarkSelected);
    ui->splitter->setStretchFactor(1, 1);
}

MainWindow::~MainWindow()
{
    while(ui->tabWidget->count() > 0)closeTab(ui->tabWidget->currentIndex());
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

void MainWindow::tabChanged()
{
    QPdfView* pdfView = qobject_cast<QPdfView*>(ui->tabWidget->currentWidget());
    if(pdfView)
    {
        if(ui->treeView->model())
            ui->treeView->model()->deleteLater();
        QPdfBookmarkModel* bookmarkModel = new QPdfBookmarkModel(ui->treeView);
        bookmarkModel->setDocument(pdfView->document());
        ui->treeView->setModel(bookmarkModel);
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
            pdfView->setPageMode(QPdfView::PageMode::MultiPage);
            ui->tabWidget->addTab(pdfView, document->metaData(QPdfDocument::MetaDataField::Title).toString());
        }
    }
}

void MainWindow::bookmarkSelected(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    QPdfView* pdfView = qobject_cast<QPdfView*>(ui->tabWidget->currentWidget());
    if(pdfView)
    {
        pdfView->pageNavigator()->jump(index.data(int(QPdfBookmarkModel::Role::Page)).toInt(), {}, index.data(int(QPdfBookmarkModel::Role::Level)).toInt());
    }
}
