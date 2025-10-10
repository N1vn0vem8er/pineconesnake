#include "previewpage.h"
#include <QDesktopServices>

PreviewPage::PreviewPage(QObject *parent) : QWebEnginePage(parent) {}

bool PreviewPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame)
{
    if(url.scheme() == QString("qrc"))
        return true;
    QDesktopServices::openUrl(url);
    return false;
}
