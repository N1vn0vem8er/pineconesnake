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
    QList<GitFileStatus> addedInIndex;
    QList<GitFileStatus> addedInWorkingDirectory;
    QList<GitFileStatus> deletedFromWorkingDirectory;
    QList<GitFileStatus> deletedFromIndex;
    GitFileStatusModel* addedModel {nullptr};
    GitFileStatusModel* changedModel {nullptr};
    GitFileStatusModel* untrackedModel {nullptr};
    void setVisibility(bool val);
    void readStatus();
    QList<GitFileStatus> getFilesStatus(const QRegularExpression& regex, const QString& results, const QString& status) const;
    QList<QPair<QString, QPair<QString, QString>>> readDiff();
    void applyDiff(QList<GitFileStatus> &files, QList<QPair<QString, QPair<QString, QString> > > &diffs);

private slots:
    void refresh();
    void gitAdd(const QModelIndex& index);
    void gitAddUntracked(const QModelIndex& index);
    void gitDiff(const QModelIndex& index);
    void gitDiffAdded(const QModelIndex& index);
    void openAdded(const QModelIndex& index);
    void openChanged(const QModelIndex& index);
    void openUntracked(const QModelIndex& index);

signals:
    void openInEditor(const QString& text, const QString& title);
    void openFile(const QString& absolutePath);
};

#endif // GITWIDGET_H
