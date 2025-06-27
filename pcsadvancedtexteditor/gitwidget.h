#ifndef GITWIDGET_H
#define GITWIDGET_H

#include <QWidget>

namespace Ui {
class GitWidget;
}

class GitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GitWidget(QWidget *parent = nullptr);
    ~GitWidget();
    void setRepositoryPath(const QString& path);
    void noRepo();

private:
    Ui::GitWidget *ui;
    QString repoPath;
    QStringList untrackedFiles;
    QStringList modifiedInWorkingDirectory;
    QStringList modifiedInIndex;
    QStringList modifiedInBoth;
    QStringList addedInIndex;
    QStringList deletedFromIndex;
    void setVisibility(bool val);
    void readStatus();
    QStringList getFilesStatus(const QRegularExpression& regex, const QString& results);
};

#endif // GITWIDGET_H
