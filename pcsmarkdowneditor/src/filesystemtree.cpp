#include "filesystemtree.h"
#include "textinputdialog.h"
#include <qdialog.h>
#include <qfuturewatcher.h>
#include <qmenu.h>
#include <fstream>
#include <qmimedata.h>
#include <qmimedatabase.h>
#include <QFuture>
#include <qtconcurrentrun.h>

FileSystemTree::FileSystemTree(QWidget *parent) : QTreeView(parent)
{
    hideColumn(1);
    hideColumn(2);
    hideColumn(3);
    hideColumn(4);
    connect(this, &QTreeView::customContextMenuRequested, this, &FileSystemTree::openContextMenu);
    connect(this, &QTreeView::doubleClicked, this, &FileSystemTree::openFilePressed);
    connect(&watcher, &QFutureWatcher<QString>::finished, this, [&]{apps = watcher.result();});
    setContextMenuPolicy(Qt::CustomContextMenu);
    startSearch();
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

QString FileSystemTree::getSelectedItem(const QModelIndex& index) const
{
    return model->filePath(index);
}

bool FileSystemTree::getHasGitRepository() const
{
    return hasGitRepository;
}

void FileSystemTree::setHasGitRepository(bool newHasGitRepository)
{
    hasGitRepository = newHasGitRepository;
}

void FileSystemTree::openOnFileContextMenu(const QString& path)
{
    if(contextMenu)
        contextMenu->deleteLater();
    contextMenu = new QMenu(this);
    QAction* openAction = new QAction(tr("Open"), contextMenu);
    connect(openAction, &QAction::triggered, this, &FileSystemTree::openFilePressed);
    contextMenu->addAction(openAction);
    QMenu* openInMenu = new QMenu(contextMenu);
    openInMenu->setTitle(tr("Open In"));

    QMimeDatabase db;
    QMimeType type = db.mimeTypeForFile(path);
    for(const auto& i : std::as_const(apps))
    {
        if(i.mimeTypes.contains(type.name()))
        {
            QAction* action = new QAction(openInMenu);
            action->setText(i.name);
            action->setIcon(QIcon::fromTheme(i.icon));
            connect(action, &QAction::triggered, this, [this, path, i]{openIn(i.exec, path);});
            openInMenu->addAction(action);
        }

    }

    contextMenu->addMenu(openInMenu);
    QAction* renameAction = new QAction(tr("Rename"), contextMenu);
    connect(renameAction, &QAction::triggered, this, &FileSystemTree::renamePressed);
    contextMenu->addAction(renameAction);
    if(hasGitRepository)
    {
        QAction* gitAddAction = new QAction(tr("Git Add"), contextMenu);
        connect(gitAddAction, &QAction::triggered, this, &FileSystemTree::addToGitRepository);
        contextMenu->addAction(gitAddAction);
        QAction* gitDiffAction = new QAction(tr("Git Diff"), contextMenu);
        connect(gitDiffAction, &QAction::triggered, this, &FileSystemTree::gitDiffPressed);
        contextMenu->addAction(gitDiffAction);
    }
}

void FileSystemTree::openOnDirContextMenu(const QString& path)
{
    if(contextMenu)
        contextMenu->deleteLater();
    contextMenu = new QMenu(this);
    QAction* openAction = new QAction(tr("Open"), contextMenu);
    connect(openAction, &QAction::triggered, this, &FileSystemTree::openDirPressed);
    contextMenu->addAction(openAction);
    QMenu* openInMenu = new QMenu(contextMenu);
    openInMenu->setTitle(tr("Open In"));

    QMimeDatabase db;
    QMimeType type = db.mimeTypeForFile(path);
    for(const auto& i : std::as_const(apps))
    {
        if(i.mimeTypes.contains(type.name()))
        {
            QAction* action = new QAction(openInMenu);
            action->setText(i.name);
            action->setIcon(QIcon::fromTheme(i.icon));
            connect(action, &QAction::triggered, this, [this, path, i]{openIn(i.exec, path);});
            openInMenu->addAction(action);
        }
    }

    contextMenu->addMenu(openInMenu);
    QAction* createFileAction = new QAction(tr("New File"), contextMenu);
    connect(createFileAction, &QAction::triggered, this, &FileSystemTree::createFile);
    contextMenu->addAction(createFileAction);
    QAction* createDirAction = new QAction(tr("New Folder"), contextMenu);
    connect(createDirAction, &QAction::triggered, this, &FileSystemTree::createDir);
    contextMenu->addAction(createDirAction);
    if(hasGitRepository)
    {
        QAction* gitAddAction = new QAction(tr("Git Add"), contextMenu);
        connect(gitAddAction, &QAction::triggered, this, &FileSystemTree::addToGitRepository);
        contextMenu->addAction(gitAddAction);
        QAction* gitDiffAction = new QAction(tr("Git Diff"), contextMenu);
        connect(gitDiffAction, &QAction::triggered, this, &FileSystemTree::gitDiffPressed);
        contextMenu->addAction(gitDiffAction);
    }
}

void FileSystemTree::openAnywhereContextMenu()
{
    if(contextMenu)
        contextMenu->deleteLater();
    contextMenu = new QMenu(this);
    QAction* createFileAction = new QAction(tr("New File"), contextMenu);
    connect(createFileAction, &QAction::triggered, this, &FileSystemTree::createFileInRoot);
    contextMenu->addAction(createFileAction);
    QAction* createDirAction = new QAction(tr("New Folder"), contextMenu);
    connect(createDirAction, &QAction::triggered, this, &FileSystemTree::createDirInRoot);
    contextMenu->addAction(createDirAction);
}

void FileSystemTree::startSearch()
{
    QFuture<QList<FileSystemTree::MimeApplication>> future = QtConcurrent::run([]{
        auto find = [](const QString& path){
            QList<FileSystemTree::MimeApplication> ret;
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
                            FileSystemTree::MimeApplication app;
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
                                auto match = QRegularExpression("\\bName=(\\w+)").match(text);
                                if(match.hasMatch())
                                    app.name = match.captured(1);
                                if(!app.name.isEmpty())
                                {
                                    auto match = QRegularExpression("\\bIcon=(\\w+)").match(text);
                                    if(match.hasMatch())
                                        app.icon = match.captured(1);
                                    auto matchExec = QRegularExpression("\\bExec=(\\w+)").match(text);
                                    if(matchExec.hasMatch())
                                        app.exec = matchExec.captured(1);
                                    ret.append(app);
                                }
                            }
                        }
                    }
                    file.close();
                }
            }
            return ret;
        };
        const QString generalPath = "/usr/share/applications";
        const QString userPath = QDir::homePath() + "/.local/share/applications";
        QList<FileSystemTree::MimeApplication> apps;
        apps.append(find(generalPath));
        const auto user = find(userPath);
        for(const auto& i : user)
        {
            if(std::find_if(apps.begin(), apps.end(), [&i](const auto& val){return i.name == val.name;}) == apps.end())

            {
                apps.append(i);
        }
        }
        return apps;
    });
    watcher.setFuture(future);
}

void FileSystemTree::openContextMenu(const QPoint& point)
{
    const auto index = indexAt(point);
    if(!index.isValid())
        openAnywhereContextMenu();
    else
    {
        const auto path = getSelectedItem(index);
        if(QFileInfo(path).isFile())
            openOnFileContextMenu(path);
        else if(QFileInfo(path).isDir())
            openOnDirContextMenu(path);
        else
            return;
    }
    contextMenu->popup(mapToGlobal(point));
}

void FileSystemTree::createFile() const
{
    if(selectionModel()->selectedIndexes().count() > 0)
    {
        const QString path = getSelectedItem(selectionModel()->selectedIndexes()[0]);
        if(QFileInfo(path).isDir())
        {
            TextInputDialog* dialog = new TextInputDialog();
            if(dialog->exec() == QDialog::Accepted)
            {
                const QString name = dialog->getText();
                if(!name.isEmpty())
                {
                    QFile file(path + '/' + name);
                    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
                    if(file.isOpen())
                    {
                        file.close();
                    }
                }
            }
        }
    }
}

void FileSystemTree::createDir() const
{
    if(selectionModel()->selectedIndexes().count() > 0)
    {
        const QString path = getSelectedItem(selectionModel()->selectedIndexes()[0]);
        if(QFileInfo(path).isDir())
        {
            TextInputDialog* dialog = new TextInputDialog();
            if(dialog->exec() == QDialog::Accepted)
            {
                const QString name = dialog->getText();
                if(!name.isEmpty())
                {
                    QDir().mkdir(path + '/' + name);
                }
            }
        }
    }
}

void FileSystemTree::createDirInRoot() const
{
    const QString path = model->rootPath();
    if(QFileInfo(path).isDir())
    {
        TextInputDialog* dialog = new TextInputDialog();
        if(dialog->exec() == QDialog::Accepted)
        {
            const QString name = dialog->getText();
            if(!name.isEmpty())
            {
                QDir().mkdir(path + '/' + name);
            }
        }
    }
}

void FileSystemTree::createFileInRoot() const
{
    const QString path = model->rootPath();
    if(QFileInfo(path).isDir())
    {
        TextInputDialog* dialog = new TextInputDialog();
        if(dialog->exec() == QDialog::Accepted)
        {
            const QString name = dialog->getText();
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

void FileSystemTree::addToGitRepository()
{
    if(selectionModel()->selectedIndexes().count() > 0)
    {
        const QString path = getSelectedItem(selectionModel()->selectedIndexes()[0]);
        if(QFileInfo(path).isFile() && hasGitRepository)
        {
            emit gitAdd(path);
        }
    }
}

void FileSystemTree::openIn(const QString& exec, const QString& path) const
{
    std::system(QString("%1 %2").arg(exec, path).toStdString().c_str());
}

void FileSystemTree::openFilePressed()
{
    if(selectionModel()->selectedIndexes().count() > 0)
    {
        const QString path = getSelectedItem(selectionModel()->selectedIndexes()[0]);
        if(QFileInfo(path).isFile())
        {
            emit openFile(path);
        }
    }
}

void FileSystemTree::openDirPressed()
{
    if(selectionModel()->selectedIndexes().count() > 0)
    {
        const QString path = getSelectedItem(selectionModel()->selectedIndexes()[0]);
        if(QFileInfo(path).isDir())
        {
            open(path);
        }
    }
}

void FileSystemTree::renamePressed()
{
    if(selectionModel()->selectedIndexes().count() > 0)
    {
        const QString path = getSelectedItem(selectionModel()->selectedIndexes()[0]);
        const QFileInfo fileInfo(path);
        TextInputDialog* dialog = new TextInputDialog(this);
        if(dialog->exec() == QDialog::Accepted)
        {
            const QString newName = dialog->getText();
            if(!newName.isEmpty())
            {
                const QString newPath = fileInfo.absolutePath() + QDir::separator() + newName;
                QFile::rename(path, newPath);
            }
        }
    }
}

void FileSystemTree::gitDiffPressed()
{
    if(selectionModel()->selectedIndexes().count() > 0)
    {
        const QString path = getSelectedItem(selectionModel()->selectedIndexes()[0]);
        if(QFileInfo(path).isFile() && hasGitRepository)
        {
            emit gitDiff(path);
        }
    }
}

void FileSystemTree::mimeSearchResultsReady(QList<MimeApplication> resoults)
{
    apps = resoults;
}


