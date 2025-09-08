#include "mainwindow.h"
#include "pdfview.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPdfBookmarkModel>
#include <QPdfDocument>
#include <qpdfpagenavigator.h>
#include <QSpacerItem>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    openButton = new QPushButton(tr("Open"), ui->toolBar);
    ui->toolBar->addWidget(openButton);
    pageSelector = new QPdfPageSelector(ui->toolBar);
    ui->toolBar->addWidget(pageSelector);

    openedFileLabel = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(openedFileLabel);

    connect(openButton, &QPushButton::clicked, this, &MainWindow::open);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionClose, &QAction::triggered, this, [&]{closeTab(ui->tabWidget->currentIndex());});
    connect(ui->actionClose_All, &QAction::triggered, this, [&]{while(ui->tabWidget->count() > 0)closeTab(ui->tabWidget->currentIndex());});
    connect(ui->actionClose_All_But_This, &QAction::triggered, this, [&]{for(int i=ui->tabWidget->count()-1; i >= 0;--i)if(i != ui->tabWidget->currentIndex())closeTab(i);});
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);
    connect(ui->navigationTreeView, &QAbstractItemView::activated, this, &MainWindow::bookmarkSelected);
    connect(ui->actionShowSidePanel, &QAction::triggered, this, [&](bool checked){ui->sideTabWidget->setVisible(checked);});
    connect(ui->actionZoom_in, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoom_out, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(pageSelector, &QPdfPageSelector::currentPageChanged, this, &MainWindow::pageSelected);
    connect(ui->thumbnailsView, &QListView::clicked, this, [&](const QModelIndex& index){pageSelected(index.row());});
    ui->splitter->setStretchFactor(1, 1);
    ui->actionShowSidePanel->setChecked(true);
    ui->thumbnailsView->setStyleSheet("background-color: white; color: black;");
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
    if(ui->tabWidget->count() == 0)
    {
        if(ui->navigationTreeView->model())
            ui->navigationTreeView->model()->deleteLater();
        openedFileLabel->clear();
    }
    PdfView* pdfView = qobject_cast<PdfView*>(ui->tabWidget->currentWidget());
    if(pdfView)
    {
        if(ui->navigationTreeView->model())
            ui->navigationTreeView->model()->deleteLater();
        QPdfBookmarkModel* bookmarkModel = new QPdfBookmarkModel(ui->navigationTreeView);
        bookmarkModel->setDocument(pdfView->document());
        ui->navigationTreeView->setModel(bookmarkModel);
        pageSelector->setDocument(pdfView->document());
        ui->thumbnailsView->setModel(pdfView->document()->pageModel());
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
            pageSelector->setDocument(document);
            pdfView->setDocument(document);
            pdfView->setPageMode(PdfView::PageMode::MultiPage);
            ui->tabWidget->addTab(pdfView, document->metaData(QPdfDocument::MetaDataField::Title).toString());
            connect(pdfView->pageNavigator(), &QPdfPageNavigator::currentPageChanged, pageSelector, &QPdfPageSelector::setCurrentPage);
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

void MainWindow::pageSelected(int index)
{
    PdfView* pdfView = qobject_cast<PdfView*>(ui->tabWidget->currentWidget());
    if(pdfView)
    {
        auto nav = pdfView->pageNavigator();
        nav->jump(index, {}, nav->currentZoom());
    }
}
