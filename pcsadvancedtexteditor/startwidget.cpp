#include "startwidget.h"
#include "settings.h"
#include "ui_startwidget.h"
#include <qdir.h>

StartWidget::StartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartWidget)
{
    ui->setupUi(this);
    connect(ui->newFileButton, &QPushButton::clicked, this, [&]{emit newFile();});
    connect(ui->openFileButton, &QPushButton::clicked, this, [&]{emit openFile();});
    connect(ui->openDirButton, &QPushButton::clicked, this, [&]{emit openDir();});
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
