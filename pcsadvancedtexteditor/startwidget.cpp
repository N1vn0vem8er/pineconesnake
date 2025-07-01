#include "startwidget.h"
#include "settings.h"
#include "ui_startwidget.h"
#include <qdir.h>
#include <qmessagebox.h>

StartWidget::StartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartWidget)
{
    ui->setupUi(this);
    connect(ui->newFileButton, &QPushButton::clicked, this, [&]{emit newFile();});
    connect(ui->openFileButton, &QPushButton::clicked, this, [&]{emit openFile();});
    connect(ui->openDirButton, &QPushButton::clicked, this, [&]{emit openDir();});
    connect(ui->recentFilesView, &QAbstractItemView::doubleClicked, this, &StartWidget::openFileFromRecentPressed);
    connect(ui->recentDirsView, &QAbstractItemView::doubleClicked, this, &StartWidget::openDirFromRecentPressed);
    connect(ui->clearRecentButton, &QPushButton::clicked, this, &StartWidget::clearRecentFiles);
    connect(ui->clearRecentDirsButton, &QPushButton::clicked, this, &StartWidget::clearRecentDirs);

    loadRecentFiles();
    loadRecentDirs();
}

StartWidget::~StartWidget()
{
    delete ui;
}

void StartWidget::refresh()
{
    loadRecentFiles();
    loadRecentDirs();
}

void StartWidget::loadRecentFiles()
{
    if(recentFilesModel == nullptr)
    {
        delete recentFilesModel;
    }
    recentFilesModel = new QStringListModel(ui->recentFilesView);
    QStringList recent;
    if(!QDir(Settings::storagePath).exists())
    {
        QDir().mkpath(Settings::storagePath);
    }
    QFile file(Settings::recentFilesStoragePath);
    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            const QString line = stream.readLine();
            if(!line.isEmpty())
                recent.append(line);
        }
        file.close();
    }
    recentFilesModel->setStringList(recent);
    ui->recentFilesView->setModel(recentFilesModel);
}

void StartWidget::loadRecentDirs()
{
    if(recentDirsModel == nullptr)
    {
        delete recentDirsModel;
    }
    recentDirsModel = new QStringListModel(ui->recentDirsView);
    QStringList recent;
    if(!QDir(Settings::storagePath).exists())
    {
        QDir().mkpath(Settings::storagePath);
    }
    QFile file(Settings::recentDirsStoragePath);
    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            const QString line = stream.readLine();
            if(!line.isEmpty())
                recent.append(line);
        }
        file.close();
    }
    recentDirsModel->setStringList(recent);
    ui->recentDirsView->setModel(recentDirsModel);
}

void StartWidget::clearRecentFiles()
{
    auto reply = QMessageBox::question(this, tr("Are you sure?"), tr("Clear recent files?"));
    if(reply == QMessageBox::Yes)
    {
        QFile file(Settings::recentFilesStoragePath);
        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
        if(file.isOpen())
        {
            file.write("");
            file.close();
        }
        loadRecentFiles();
        emit recentFilesCleared();
    }
}

void StartWidget::clearRecentDirs()
{
    auto reply = QMessageBox::question(this, tr("Are you sure?"), tr("Clear recent directories?"));
    if(reply == QMessageBox::Yes)
    {
        QFile file(Settings::recentDirsStoragePath);
        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
        if(file.isOpen())
        {
            file.write("");
            file.close();
        }
        loadRecentDirs();
        emit recentDirsCleared();
    }
}

void StartWidget::openFileFromRecentPressed(const QModelIndex& index)
{
    emit openFileFromRecent(recentFilesModel->stringList().at(index.row()));
}

void StartWidget::openDirFromRecentPressed(const QModelIndex& index)
{
    emit openDirFromRecent(recentDirsModel->stringList().at(index.row()));
}
