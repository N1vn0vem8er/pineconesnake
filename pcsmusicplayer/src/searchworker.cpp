#include "searchworker.h"
#include <QDir>
#include <QProcess>
#include <taglib/fileref.h>
#include <taglib/tag.h>

SearchWorker::SearchWorker(QObject *parent)
    : QObject{parent}
{}

void SearchWorker::doWork(const QStringList &searchPaths, const QStringList &paths)
{
    QList<Track> tracks;
    for(const auto& path : searchPaths)
    {
        QProcess process;
        process.start("find", {path, "-type", "f", "-name", "*.mp3"});
        process.waitForStarted(-1);
        process.waitForFinished(-1);
        process.waitForReadyRead(-1);
        QTextStream resoult(process.readAllStandardOutput());
        while(!resoult.atEnd())
        {
            QString path = resoult.readLine();
            if(!paths.contains(path))
            {
                TagLib::FileRef f(path.toStdString().c_str());
                if(!f.isNull() && f.tag())
                {
                    tracks.append(Track(-1, QString::fromStdString(f.tag()->title().toCString()), path, QString::fromStdString(f.tag()->artist().toCString()),
                                        QString::fromStdString(f.tag()->album().toCString()), f.tag()->track(), f.audioProperties()->lengthInSeconds(), 0, false));
                }
            }
        }
    }
    emit resultReady(tracks);
    emit finished();
}
