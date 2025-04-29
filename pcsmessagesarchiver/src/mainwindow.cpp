#include "mainwindow.h"
#include "browsemessageswidget.h"
#include "homewidget.h"
#include "managecontactswidget.h"
#include "resourcesmanager.h"
#include "savemessagewidget.h"
#include "settings.h"
#include "settingswidget.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QStyleFactory>

#define VERSION "1.0.1"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->homeButton, &QPushButton::pressed, this, &MainWindow::addNewHomeWidget);
    connect(ui->saveMessageButton, &QPushButton::pressed, this, [this]{addTab(new SaveMessageWidget(this), tr("Save Message"));});
    connect(ui->browseMessagesButton, &QPushButton::pressed, this, [this]{addTab(new BrowseMessagesWidget(this), tr("Messages"));});
    connect(ui->settingsButton, &QPushButton::pressed, this, [this]{addTab(new SettingsWidget(this), tr("Settings"));});
    connect(qApp, &QApplication::aboutToQuit, this, []{ResourcesManager::getInstance()->close();});
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::removeTab);
    connect(ui->aboutButton, &QPushButton::clicked, this, &MainWindow::openAboutWidget);
    connect(ui->aboutQtButton, &QPushButton::clicked, this, [this]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->manageContactsButton, &QPushButton::clicked, this, [this]{addTab(new ManageContactsWidget(this), tr("Manage Contacts"));});
    Settings settings;
    settings.loadSettings();
    ui->splitter->setStretchFactor(1, Settings::mainSplitterStretchFactor);
    ResourcesManager* rm = ResourcesManager::getInstance();
    rm->loadMessagesTitles();
    addNewHomeWidget();
    if(QStyleFactory::keys().contains(Settings::theme))
    {
        QApplication::setStyle(Settings::theme);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTab(QWidget *tab, const QString& title)
{
    ui->tabWidget->addTab(tab, title);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->currentIndex()+1);
}

void MainWindow::removeTab(int index)
{
    QWidget* w = ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    delete w;
}

void MainWindow::addNewHomeWidget()
{
    HomeWidget* widget = new HomeWidget(this);
    connect(widget, &HomeWidget::openMessageFromRecent, this, &MainWindow::openFromRecent);
    addTab(widget, tr("Home"));
}

void MainWindow::openFromRecent(int id)
{
    BrowseMessagesWidget* widget = new BrowseMessagesWidget(this);
    widget->openById(id);
    addTab(widget, tr("Messages"));
}

void MainWindow::openAboutWidget()
{
    QMessageBox::about(this, tr("About Messages Archiver"),
                       tr("<html><body><h3>PCS Messages Archiver</h3><p>PSC Messages Archiver is an application for manual archiving of messages such as emails. It is a part of Pinecone Snake project.</p><p>Version: %1</p><p>License: <a href=\"&%2\">GPL 3</a></p></body></html>").arg(VERSION).arg(LICENSELINK));
}
