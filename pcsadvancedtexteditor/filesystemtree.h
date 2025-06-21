#ifndef FILESYSTEMTREE_H
#define FILESYSTEMTREE_H

#include <QFileSystemModel>
#include <QTreeView>

class FileSystemTree : public QTreeView
{
    Q_OBJECT
public:
    FileSystemTree(QWidget* parent = nullptr);
    ~FileSystemTree();
    void open(const QString &path);
    void setDirectory(const QString &path);
    QString getSelectedItem(const QModelIndex &index);

private:
    QFileSystemModel* model {nullptr};
    QMenu* contextMenu {nullptr};
    void openOnFileContextMenu(const QString &path);
    void openOnDirContextMenu(const QString &path);
    void openAnywhereContextMenu();

private slots:
    void openContextMenu(const QPoint &point);
    void createFile();
    void createDir();
    void addToGitRepository();
    void openIn();

signals:
    void openFile(const QString& path);
};

#endif // FILESYSTEMTREE_H
