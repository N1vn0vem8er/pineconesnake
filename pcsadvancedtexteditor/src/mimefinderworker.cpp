#include "mimefinderworker.h"
#include "globals.h"
#include <QDir>
#include <QRegularExpressionMatchIterator>

MimeFinderWorker::MimeFinderWorker(QObject *parent)
    : QObject{parent}
{}

void MimeFinderWorker::start()
{
    const QString generalPath = "/usr/share/applications";
    const QString userPath = QDir::homePath() + "/.local/share/applications";
    Globals::apps.append(find(generalPath));
    const auto user = find(userPath);
    for(const auto& i : user)
    {
        if(std::find_if(Globals::apps.begin(), Globals::apps.end(), [&i](const auto& val){return i.name == val.name;}) == Globals::apps.end())
        {
            Globals::apps.append(i);
        }
    }
    emit finished();
}

QList<MimeApplication> MimeFinderWorker::find(const QString &path)
{
    QList<MimeApplication> ret;
    const auto dirs = QDir(path).entryList(QStringList() << "*.desktop", QDir::Files);
    for(const auto& desktopFile : dirs)
    {
        QFile file(path + "/" + desktopFile);
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            const QString text = file.readAll();
            QRegularExpressionMatchIterator i = QRegularExpression(R"(MimeType=(.*))").globalMatch(text);
            while(i.hasNext())
            {
                QRegularExpressionMatch match = i.next();
                if(match.hasMatch())
                {
                    MimeApplication app;
                    for(int i = 1; i<match.capturedLength(); i++)
                    {
                        if(!match.captured(i).isEmpty())
                        {
                            auto tmp = match.captured(i).split(";");
                            tmp.removeAll(QString(""));
                            app.mimeTypes.append(tmp);
                        }
                    }
                    if(!app.mimeTypes.isEmpty())
                    {
                        auto match = QRegularExpression("\\bName=(\\w+)").match(text);
                        if(match.hasMatch())
                            app.name = match.captured(1);
                        if(!app.name.isEmpty())
                        {
                            auto match = QRegularExpression("\\bIcon=(\\w+)").match(text);
                            if(match.hasMatch())
                                app.icon = match.captured(1);
                            auto matchExec = QRegularExpression("\\bExec=(\\w+)").match(text);
                            if(matchExec.hasMatch())
                                app.exec = matchExec.captured(1);
                            ret.append(app);
                        }
                    }
                }
            }
            file.close();
        }
    }
    return ret;
}
