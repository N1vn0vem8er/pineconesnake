#include "filesystemtree.h"
#include "textinputdialog.h"
#include <qdialog.h>
#include <qmenu.h>
#include <fstream>

FileSystemTree::FileSystemTree(QWidget *parent) : QTreeView(parent)
{
    hideColumn(1);
    hideColumn(2);
    hideColumn(3);
    hideColumn(4);
    contextMenu = new QMenu(this);
    QAction* createFileAction = new QAction(tr("New File"), contextMenu);
    connect(createFileAction, &QAction::triggered, this, &FileSystemTree::createFile);
    contextMenu->addAction(createFileAction);
    QAction* createDirAction = new QAction(tr("New Folder"), contextMenu);
    connect(createDirAction, &QAction::triggered, this, &FileSystemTree::createDir);
    contextMenu->addAction(createDirAction);
    QAction* gitAddAction = new QAction(tr("Git Add"), contextMenu);
    connect(gitAddAction, &QAction::triggered, this, &FileSystemTree::addToGitRepository);
    contextMenu->addAction(gitAddAction);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &FileSystemTree::openContextMenu);
}

FileSystemTree::~FileSystemTree()
{

}

void FileSystemTree::open(const QString& path)
{
    model = new QFileSystemModel(this);
    model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    model->setRootPath(path);
    setModel(model);
    setRootIndex(model->index(path));
}

void FileSystemTree::setDirectory(const QString& path)
{
    if(model != NULL)
        setRootIndex(model->index(path));
}

QString FileSystemTree::getSelectedItem(const QModelIndex& index)
{
    return model->filePath(index);
}

void FileSystemTree::openContextMenu(const QPoint& point)
{
    contextMenu->popup(mapToGlobal(point));
}

void FileSystemTree::createFile()
{
    if(selectionModel()->selectedIndexes().count() > 0)
    {
        QString path = getSelectedItem(selectionModel()->selectedIndexes()[0]);
        if(QFileInfo(path).isDir())
        {
            TextInputDialog* dialog = new TextInputDialog();
            if(dialog->exec() == QDialog::Accepted)
            {
                QString name = dialog->getText();
                if(!name.isEmpty())
                {
                    std::ofstream file;
                    file.open(path.toStdString() + '/' + name.toStdString());
                    if(file.is_open())
                    {
                        file.close();
                    }
                }
            }
        }
    }
}

void FileSystemTree::createDir()
{
    if(selectionModel()->selectedIndexes().count() > 0)
    {
        QString path = getSelectedItem(selectionModel()->selectedIndexes()[0]);
        if(QFileInfo(path).isDir())
        {
            TextInputDialog* dialog = new TextInputDialog();
            if(dialog->exec() == QDialog::Accepted)
            {
                QString name = dialog->getText();
                if(!name.isEmpty())
                {
                    QDir().mkdir(path + '/' + name);
                }
            }
        }
    }
}

void FileSystemTree::addToGitRepository()
{

}


