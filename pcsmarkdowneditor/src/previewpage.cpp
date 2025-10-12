#include "previewpage.h"
#include <QDesktopServices>
#include <qdir.h>

PreviewPage::PreviewPage(QObject *parent) : QWebEnginePage(parent) {}

QString PreviewPage::getAbsolutePath() const
{
    return absolutePath;
}

void PreviewPage::setAbsolutePath(const QString &newAbsolutePath)
{
    absolutePath = newAbsolutePath;
}

bool PreviewPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame)
{
    const QString path = QFileInfo(absolutePath).absoluteDir().absolutePath() + QDir::separator() + url.toString().remove("qrc:pages/");
    if(QFile::exists(path))
    {
        setAbsolutePath(path);
        emit openedFile(path);
        return false;
    }
    if(url.scheme() == QString("qrc"))
        return true;
    QDesktopServices::openUrl(url);
    return false;
}
