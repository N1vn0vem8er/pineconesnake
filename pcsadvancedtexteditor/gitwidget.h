#ifndef GITWIDGET_H
#define GITWIDGET_H

#include "gitfilestatusmodel.h"
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
    QList<GitFileStatus> untrackedFiles;
    QList<GitFileStatus> modifiedInWorkingDirectory;
    QList<GitFileStatus> modifiedInIndex;
    QList<GitFileStatus> modifiedInBoth;
    QList<GitFileStatus> addedInIndex;
    QList<GitFileStatus> deletedFromIndex;
    GitFileStatusModel* addedModel {nullptr};
    void setVisibility(bool val);
    void readStatus();
    QList<GitFileStatus> getFilesStatus(const QRegularExpression& regex, const QString& results, const QString& status);
};

#endif // GITWIDGET_H
