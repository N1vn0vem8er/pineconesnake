#include "mainwindow.h"
#include "searchworker.h"
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 1);
}

MainWindow::~MainWindow()
{
    if(searchThread != nullptr)
    {
        searchThread->requestInterruption();
        searchThread->wait();
    }
    delete ui;
}

void MainWindow::startMusicSearch()
{
    if(searchThread != nullptr)
    {
        searchThread->requestInterruption();
        searchThread->wait();
    }
    searchThread = new QThread();
    SearchWorker* worker = new SearchWorker;
    worker->moveToThread(searchThread);
    connect(searchThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::startSearch, worker, &SearchWorker::doWork);
    connect(worker, &SearchWorker::resultReady, this, &MainWindow::searchResoultsReady);

}

void MainWindow::searchResoultsReady(const QList<Track> tracks)
{

}
