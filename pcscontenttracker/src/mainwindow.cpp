#include "mainwindow.h"
#include "itemwidget.h"
#include "qprocess.h"
#include "ui_mainwindow.h"
#include "resourcemanager.h"
#include "addnewdialog.h"
#include "settings.h"
#include "editdialog.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#define VERSION "1.0.2"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addNewItem);
    connect(ui->currentButton, &QPushButton::clicked, this, &MainWindow::displayCurrent);
    connect(ui->finishedButton, &QPushButton::clicked, this, &MainWindow::displayFinished);
    connect(ui->plannedButton, &QPushButton::clicked, this, &MainWindow::displayPlanned);
    connect(ui->allButton, &QPushButton::clicked, this, &MainWindow::displayAll);
    connect(ui->searchBar, &QLineEdit::textChanged, this, &MainWindow::search);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::addNewItem);
    connect(ui->actionFinished, &QAction::triggered, this, &MainWindow::displayFinished);
    connect(ui->actionPlanned, &QAction::triggered, this, &MainWindow::displayPlanned);
    connect(ui->actionCurrent, &QAction::triggered, this, &MainWindow::displayCurrent);
    connect(ui->actionAll, &QAction::triggered, this, &MainWindow::displayAll);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(qApp, &QApplication::lastWindowClosed, this, []{ResourceManager::getInstance()->close();});
    connect(ui->actionOpen_images_directory, &QAction::triggered, this, &MainWindow::openImagesDir);
    connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::refresh);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [this]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_Content_Tracker, &QAction::triggered, this, &MainWindow::openAbout);
    refresh();
    QDir dir(Settings::imagesPath);
    if(!dir.exists())
    {
        dir.mkpath(".");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    switch(selected)
    {
    case SelectedModes::All:
        displayAll();
        break;
    case SelectedModes::Finished:
        displayFinished();
        break;
    case SelectedModes::Planned:
        displayPlanned();
        break;
    case SelectedModes::Current:
        displayCurrent();
        break;
    }
}

void MainWindow::clearLayout()
{
    if(layout)
    {
        QLayoutItem* layoutItem;
        while((layoutItem = layout->takeAt(0)))
        {
            delete layoutItem->widget();
            delete layoutItem;
        }
        delete layout;
        layout = nullptr;
    }
}

void MainWindow::addNewItem()
{
    AddNewDialog* dialog = new AddNewDialog(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        Item item = dialog->getItem();
        ResourceManager* rm = ResourceManager::getInstance();
        rm->addItem(item);
        refresh();
    }
    delete dialog;
}

void MainWindow::editItem(Item& item)
{
    EditDialog* dialog = new EditDialog(item, this);
    if(dialog->exec() == QDialog::Accepted)
    {
        ResourceManager* rm = ResourceManager::getInstance();
        rm->editItem(dialog->getIten());
        refresh();
    }
}

void MainWindow::deleteItem(const Item &item)
{
    QMessageBox::StandardButton dialog = QMessageBox::question(this, tr("Delete"), tr("Are you sure you want to delete %1?").arg(item.title), QMessageBox::Yes | QMessageBox::No);
    if(dialog == QMessageBox::Yes)
    {
        ResourceManager* rm = ResourceManager::getInstance();
        rm->deleteItem(item);
        refresh();
    }
}

void MainWindow::displayFinished()
{
    clearLayout();
    layout = new QVBoxLayout(ui->titlesView);
    const QList<Item> items = ResourceManager::getInstance()->getAllFinished();
    for(const auto& i : items)
    {
        ItemWidget* widget = new ItemWidget(i, ui->titlesView);
        connect(widget, &ItemWidget::deleteItem, this, &MainWindow::deleteItem);
        connect(widget, &ItemWidget::requestRefresh, this, &MainWindow::refresh);
        layout->addWidget(widget);
    }
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding));
    ui->titlesView->setLayout(layout);
    selected = SelectedModes::Finished;
}

void MainWindow::displayPlanned()
{
    clearLayout();
    layout = new QVBoxLayout(ui->titlesView);
    const QList<Item> items = ResourceManager::getInstance()->getAllPlanned();
    for(const auto& i : items)
    {
        ItemWidget* widget = new ItemWidget(i, ui->titlesView);
        connect(widget, &ItemWidget::deleteItem, this, &MainWindow::deleteItem);
        connect(widget, &ItemWidget::editItem, this, &MainWindow::editItem);
        connect(widget, &ItemWidget::requestRefresh, this, &MainWindow::refresh);
        layout->addWidget(widget);
    }
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding));
    ui->titlesView->setLayout(layout);
    selected = SelectedModes::Planned;
}

void MainWindow::displayCurrent()
{
    clearLayout();
    layout = new QVBoxLayout(ui->titlesView);
    const QList<Item> items = ResourceManager::getInstance()->getAllCurrnet();
    for(const auto& i : items)
    {
        ItemWidget* widget = new ItemWidget(i, ui->titlesView);
        connect(widget, &ItemWidget::deleteItem, this, &MainWindow::deleteItem);
        connect(widget, &ItemWidget::editItem, this, &MainWindow::editItem);
        connect(widget, &ItemWidget::requestRefresh, this, &MainWindow::refresh);
        layout->addWidget(widget);
    }
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding));
    ui->titlesView->setLayout(layout);
    selected = SelectedModes::Current;
}

void MainWindow::displayAll()
{
    clearLayout();
    layout = new QVBoxLayout(ui->titlesView);
    const QList<Item> items = ResourceManager::getInstance()->getAllItems();
    for(const auto& i : items)
    {
        ItemWidget* widget = new ItemWidget(i, ui->titlesView);
        connect(widget, &ItemWidget::deleteItem, this, &MainWindow::deleteItem);
        connect(widget, &ItemWidget::editItem, this, &MainWindow::editItem);
        connect(widget, &ItemWidget::requestRefresh, this, &MainWindow::refresh);
        layout->addWidget(widget);
    }
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding));
    ui->titlesView->setLayout(layout);
    selected = SelectedModes::All;
}

void MainWindow::search()
{
    if(ui->searchBar->text().isEmpty())
    {
        refresh();
        return;
    }
    clearLayout();
    layout = new QVBoxLayout(ui->titlesView);
    const QList<Item> items = ResourceManager::getInstance()->get10ItemsByTitle(ui->searchBar->text());
    for(const auto& i : items)
    {
        ItemWidget* widget = new ItemWidget(i, ui->titlesView);
        connect(widget, &ItemWidget::deleteItem, this, &MainWindow::deleteItem);
        connect(widget, &ItemWidget::editItem, this, &MainWindow::editItem);
        connect(widget, &ItemWidget::requestRefresh, this, &MainWindow::refresh);
        layout->addWidget(widget);
    }
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding));
    ui->titlesView->setLayout(layout);
}

void MainWindow::openImagesDir()
{
    QProcess* process = new QProcess();
    process->execute("xdg-open", QStringList() << Settings::imagesPath);
    process->waitForStarted();
    process->waitForFinished();
    delete process;
}

void MainWindow::openAbout()
{
    QMessageBox::about(this, tr("About Content Tracker"),
                       tr(R"(
    <html>
        <body>
            <h3>PCS Content Tracker</h3>
            <p>PCS Content Tracker is an application for keeping track of shows and books you are consuming. Is is a prat of Pinecone Snake project.</p><p>Version: %1</p>
            <p>License: <a href=\"%2\">GPL 3</a></p>
        </body>
    </html>)").arg(VERSION, LICENSELINK));
}
