#ifndef PREVIEWPAGE_H
#define PREVIEWPAGE_H

#include <QWebEnginePage>

class PreviewPage : public QWebEnginePage
{
    Q_OBJECT
public:
    PreviewPage(QObject* parent = nullptr);
    QString getAbsolutePath() const;
    void setAbsolutePath(const QString &newAbsolutePath);

private:
    QString absolutePath;

protected:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override;

signals:
    void openedFile(const QString& path);
};

#endif // PREVIEWPAGE_H
