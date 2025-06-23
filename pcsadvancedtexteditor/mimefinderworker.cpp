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
    Globals::apps.append(find(userPath));
    Globals::apps.append(find(generalPath));
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
                        QRegularExpression regex("\\bName=(\\w+)");
                        auto match = regex.match(text);
                        if(match.hasMatch())
                            app.name = match.captured(1);
                        if(!app.name.isEmpty())
                            ret.append(app);
                    }
                }
            }
            file.close();
        }
    }
    return ret;
}
