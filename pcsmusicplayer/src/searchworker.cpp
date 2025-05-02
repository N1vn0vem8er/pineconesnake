#include "searchworker.h"
#include <QDir>
#include <QProcess>

SearchWorker::SearchWorker(QObject *parent)
    : QObject{parent}
{}

void SearchWorker::doWork(const QString &path)
{
    QProcess* process = new QProcess();
    process->start("find", QStringList() << path << "-type" << "f" << "-name" << "*.mp3");
    process->waitForStarted(-1);
    process->waitForFinished(-1);
    process->waitForReadyRead(-1);
    QTextStream resoult(process->readAllStandardOutput());
    QList<Track> tracks;
    while(!resoult.atEnd())
    {
        QString path = resoult.readLine();
        QFileInfo info(path);
        tracks.append(Track(1, info.fileName(), path, "test", "test", 1, 1, 1, false));
    }
    delete process;
    emit resultReady(tracks);
    emit finished();
}
