#include "mainwindow.h"
#include "pdfview.h"
#include "searchresultdelegate.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPdfBookmarkModel>
#include <QPdfDocument>
#include <qpdfpagenavigator.h>
#include <QSpacerItem>
#include <QPushButton>
#include <QPdfSearchModel>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

#define VERSION "0.1.0"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"
#define PROJECTLINK "https://github.com/N1vn0vem8er/pineconesnake"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("pcspdfviewer");
    restoreState(settings.value("state").toByteArray());
    restoreGeometry(settings.value("geometry").toByteArray());
    openButton = new QPushButton(tr("Open"), ui->toolBar);
    nextPageButton = new QPushButton(ui->toolBar);
    nextPageButton->setIcon(QIcon::fromTheme("go-next"));
    nextPageButton->setToolTip(tr("Next"));
    previousPageButton = new QPushButton(ui->toolBar);
    previousPageButton->setIcon(QIcon::fromTheme("go-previous"));
    previousPageButton->setToolTip(tr("Previous"));
    ui->toolBar->addWidget(openButton);
    ui->toolBar->addWidget(previousPageButton);
    ui->toolBar->addWidget(nextPageButton);
    pageSelector = new QPdfPageSelector(ui->toolBar);
    ui->toolBar->addWidget(pageSelector);

    searchModel = new QPdfSearchModel(this);
    ui->searchResultsView->setModel(searchModel);
    ui->searchResultsView->setItemDelegate(new SearchResultDelegate(ui->searchResultsView));

    openedFileLabel = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(openedFileLabel);

    connect(nextPageButton, &QPushButton::clicked, this, [&]{pageSelector->setCurrentPage(pageSelector->currentPage() + 1);});
    connect(previousPageButton, &QPushButton::clicked, this, [&]{pageSelector->setCurrentPage(pageSelector->currentPage() - 1);});
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
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, [&](const QString& text){searchModel->setSearchString(text);});
    connect(ui->actionFind, &QAction::triggered, this, [&]{ui->searchLineEdit->setVisible(!ui->searchLineEdit->isVisible());});
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [&]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_PCS_PdfViewer, &QAction::triggered, this, &MainWindow::openAbout);
    ui->searchLineEdit->setVisible(false);
    ui->splitter->setStretchFactor(1, 1);
    ui->actionShowSidePanel->setChecked(true);
    ui->thumbnailsView->setStyleSheet("background-color: white; color: black;");
    loadRecent();
}

MainWindow::~MainWindow()
{
    QSettings settings("pcspdfviewer");
    settings.setValue("state", saveState());
    settings.setValue("geometry", saveGeometry());
    while(ui->tabWidget->count() > 0)closeTab(ui->tabWidget->currentIndex());
    delete ui;
}

void MainWindow::loadRecent()
{
    const QString storagePath = QDir::homePath() + "/.local/share/pineconesnake/pcspdfviewer";
    ui->menuRecent_Files->clear();
    if(!QDir(storagePath).exists())
    {
        QDir().mkpath(storagePath);
    }
    QFile file(storagePath + "/recent.txt");
    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            const QString line = stream.readLine();
            if(!line.isEmpty())
            {
                QAction* action = new QAction(ui->menuRecent_Files);
                action->setText(line);
                connect(action, &QAction::triggered, this, [this, line]{openFile(line);});
                ui->menuRecent_Files->addAction(action);
            }
        }
        file.close();
    }
    if(ui->menuRecent_Files->isEmpty())
    {
        QAction* action = new QAction(tr("no recent"), ui->menuRecent_Files);
        action->setEnabled(false);
        ui->menuRecent_Files->addAction(action);
    }
}

void MainWindow::saveRecent(const QString& path)
{
    const QString storagePath = QDir::homePath() + "/.local/share/pineconesnake/pcspdfviewer/recent.txt";
    QFile file(storagePath);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    if(file.isOpen())
    {
        QStringList paths;
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            paths << stream.readLine();
        }
        if(!paths.contains(path))
        {
            file.seek(0);
            if(file.isOpen())
            {
                QTextStream stream(&file);
                if(paths.length() >= 10) paths.removeAt(0);
                paths.append(path);
                for(const auto& i : std::as_const(paths))
                {
                    stream << i + "\n";
                }
            }
        }
        file.close();
    }
}

void MainWindow::openFile(const QString& path)
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
    pdfView->setSearchModel(searchModel);
    saveRecent(path);
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
        searchModel->setDocument(pdfView->document());
    }
}

void MainWindow::open()
{
    const QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open"), QDir::homePath(), "*.pdf");
    if(!paths.isEmpty())
    {
        for(const QString& path : paths)
        {
            openFile(path);
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

void MainWindow::openAbout()
{
    QMessageBox::about(this, tr("About PCS Pdf Viewer"), tr(R"(
    <html>
        <body>
            <h3>Pdf Viewer</h3>
            <p>PCS Pdf Viewer is a pdf viewer. Is is a part of <a href="%1">Pinecone Snake</a> project.</p>
            <p>Version: %2</p>
            <p>License: <a href="%3">GPL 3</a></p>
        </body>
    </html>)").arg(PROJECTLINK, VERSION, LICENSELINK));
}
