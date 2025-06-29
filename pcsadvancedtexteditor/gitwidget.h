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
    void openGitCommit();
    void gitCommit(const QString& title, const QString& description);
    void gitPush();
    void gitPull();

signals:
    void openInEditor(const QString& text, const QString& title, bool readOnly = true, bool spellChecking = false);
    void openFile(const QString& absolutePath);
    void addTab(QWidget* widget, const QString& title);
};

#endif // GITWIDGET_H
