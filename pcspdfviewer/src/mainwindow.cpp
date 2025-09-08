#include "mainwindow.h"
#include "pdfview.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPdfBookmarkModel>
#include <QPdfDocument>
#include "pdfview.h"
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
    connect(ui->actionShow_bookmarks, &QAction::triggered, this, [&](bool checked){ui->treeView->setVisible(checked);});
    connect(ui->actionZoom_in, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoom_out, &QAction::triggered, this, &MainWindow::zoomOut);
    ui->splitter->setStretchFactor(1, 1);
    ui->actionShow_bookmarks->setChecked(true);
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
        PdfView* pdfView = qobject_cast<PdfView*>(widget);
        if(pdfView)
        {
            pdfView->document()->close();
        }
        widget->deleteLater();
    }
}

void MainWindow::tabChanged()
{
    PdfView* pdfView = qobject_cast<PdfView*>(ui->tabWidget->currentWidget());
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
            PdfView* pdfView = new PdfView(ui->tabWidget);
            connect(pdfView, &PdfView::zoomFactorChanged, this, &MainWindow::zoomChanged);
            QPdfDocument* document = new QPdfDocument(pdfView);
            document->load(path);
            pdfView->setDocument(document);
            pdfView->setPageMode(PdfView::PageMode::MultiPage);
            ui->tabWidget->addTab(pdfView, document->metaData(QPdfDocument::MetaDataField::Title).toString());
        }
    }
}

void MainWindow::bookmarkSelected(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    PdfView* pdfView = qobject_cast<PdfView*>(ui->tabWidget->currentWidget());
    if(pdfView)
    {
        pdfView->pageNavigator()->jump(index.data(int(QPdfBookmarkModel::Role::Page)).toInt(), {}, index.data(int(QPdfBookmarkModel::Role::Level)).toInt());
    }
}

void MainWindow::zoomIn()
{
    PdfView* pdfView = qobject_cast<PdfView*>(ui->tabWidget->currentWidget());
    if(pdfView)
    {
        pdfView->setZoomFactor(pdfView->zoomFactor() + 0.1);
    }
}

void MainWindow::zoomOut()
{
    PdfView* pdfView = qobject_cast<PdfView*>(ui->tabWidget->currentWidget());
    if(pdfView && pdfView->zoomFactor() - 0.1 > 0)
    {
        pdfView->setZoomFactor(pdfView->zoomFactor() - 0.1);
    }
}

void MainWindow::zoomChanged(qreal zoom)
{
    ui->statusbar->showMessage(tr("Zoom Factor: %1").arg(zoom), 500);
}
