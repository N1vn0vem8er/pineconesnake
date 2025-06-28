#include "gitwidget.h"
#include "gitfilestatusitemdelegate.h"
#include "ui_gitwidget.h"
#include <qdir.h>
#include <qfileinfo.h>
#include <qmenu.h>
#include <qprocess.h>

GitWidget::GitWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GitWidget)
{
    ui->setupUi(this);

    connect(ui->refreshButton, &QPushButton::clicked, this, &GitWidget::refresh);

    setVisibility(false);
    ui->addedView->setItemDelegate(new GitFileStatusItemDelegate(ui->addedView));
    ui->changedView->setItemDelegate(new GitFileStatusItemDelegate(ui->changedView));
    ui->untrackedView->setItemDelegate(new GitFileStatusItemDelegate(ui->untrackedView));
    QMenu* changedContextMenu = new QMenu(ui->changedView);
    QAction* gitAdd = new QAction(changedContextMenu);
    gitAdd->setText(tr("Add"));
    changedContextMenu->addAction(gitAdd);
    connect(gitAdd, &QAction::triggered, ui->changedView, &GitFilesView::gitAddPressed);
    connect(ui->changedView, &GitFilesView::gitAdd, this, &GitWidget::gitAdd);
    QAction* gitDiff = new QAction(changedContextMenu);
    gitDiff->setText(tr("Diff"));
    changedContextMenu->addAction(gitDiff);
    connect(gitDiff, &QAction::triggered, ui->changedView, &GitFilesView::gitDiffPressed);
    connect(ui->changedView, &GitFilesView::gitDiff, this, &GitWidget::gitDiff);
    QAction* openFile = new QAction(changedContextMenu);
    openFile->setText(tr("Open"));
    changedContextMenu->addAction(openFile);
    connect(openFile, &QAction::triggered, ui->changedView, &GitFilesView::openFilePressed);
    connect(ui->changedView, &GitFilesView::openFile, this, &GitWidget::openChanged);

    ui->changedView->setContextMenu(changedContextMenu);


    QMenu* untrackedContextMenu = new QMenu(ui->untrackedView);
    QAction* utGitAdd = new QAction(untrackedContextMenu);
    utGitAdd->setText(tr("Add"));
    untrackedContextMenu->addAction(utGitAdd);
    QAction* utOpenFile = new QAction(untrackedContextMenu);
    utOpenFile->setText(tr("Open"));
    untrackedContextMenu->addAction(utOpenFile);
    connect(utOpenFile, &QAction::triggered, ui->untrackedView, &GitFilesView::openFilePressed);
    connect(ui->untrackedView, &GitFilesView::openFile, this, &GitWidget::openUntracked);

    ui->untrackedView->setContextMenu(untrackedContextMenu);

    QMenu* addedContextMenu = new QMenu(ui->addedView);
    QAction* addedGitDiff = new QAction(addedContextMenu);
    addedGitDiff->setText(tr("Diff"));
    addedContextMenu->addAction(addedGitDiff);
    connect(addedGitDiff, &QAction::triggered, ui->addedView, &GitFilesView::gitDiffPressed);
    connect(ui->addedView, &GitFilesView::gitDiff, this, &GitWidget::gitDiffAdded);
    QAction* addedOpenFile = new QAction(addedContextMenu);
    addedOpenFile->setText(tr("Open"));
    addedContextMenu->addAction(addedOpenFile);
    connect(addedOpenFile, &QAction::triggered, ui->addedView, &GitFilesView::openFilePressed);
    connect(ui->addedView, &GitFilesView::openFile, this, &GitWidget::openAdded);

    ui->addedView->setContextMenu(addedContextMenu);

    connect(utGitAdd, &QAction::triggered, ui->untrackedView, &GitFilesView::gitAddPressed);
    connect(ui->untrackedView, &GitFilesView::gitAdd, this, &GitWidget::gitAddUntracked);
}

GitWidget::~GitWidget()
{
    delete ui;
}

void GitWidget::setRepositoryPath(const QString &path)
{
    repoPath = path;
    setVisibility(true);
    readStatus();
}

void GitWidget::noRepo()
{
    setVisibility(false);
    repoPath.clear();
}

void GitWidget::setVisibility(bool val)
{
    ui->gitPullButton->setVisible(val);
    ui->gitPushButton->setVisible(val);
    ui->commitButton->setVisible(val);
    ui->refreshButton->setVisible(val);
    ui->addedLabel->setVisible(val);
    ui->changedLabel->setVisible(val);
    ui->untreckedLabel->setVisible(val);
    ui->addedView->setVisible(val);
    ui->changedView->setVisible(val);
    ui->untrackedView->setVisible(val);
    ui->addedTextLabel->setVisible(val);
    ui->changedTextLabel->setVisible(val);
    ui->untrackedTextLabel->setVisible(val);
    ui->noRepoLabel->setVisible(!val);
}

void GitWidget::readStatus()
{
    QProcess* process = new QProcess(this);
    process->setWorkingDirectory(repoPath);
    process->startCommand("git status -s");
    process->waitForStarted();
    process->waitForFinished();
    process->waitForReadyRead();
    const QString results = process->readAllStandardOutput();
    untrackedFiles = getFilesStatus(QRegularExpression(R"(\?\?\s+(.*))"), results, "??");
    modifiedInIndex = getFilesStatus(QRegularExpression(R"(M.\s+(.*))"), results, "M ");
    modifiedInWorkingDirectory = getFilesStatus(QRegularExpression(R"(.M\s+(.*))"), results, " M");
    addedInIndex = getFilesStatus(QRegularExpression(R"(A.\s+(.*))"), results, "A ");
    addedInWorkingDirectory = getFilesStatus(QRegularExpression(R"(.A\s+(.*))"), results, " A");
    deletedFromIndex = getFilesStatus(QRegularExpression(R"(D.\s+(.*))"), results, "D ");
    deletedFromWorkingDirectory = getFilesStatus(QRegularExpression(R"(.D\s+(.*))"), results, " D");

    auto diff = readDiff();

    applyDiff(untrackedFiles, diff);
    applyDiff(modifiedInIndex, diff);
    applyDiff(modifiedInWorkingDirectory, diff);
    applyDiff(addedInIndex, diff);
    applyDiff(addedInWorkingDirectory, diff);
    applyDiff(deletedFromIndex, diff);
    applyDiff(deletedFromWorkingDirectory, diff);

    addedModel = new GitFileStatusModel(ui->addedView);
    addedModel->setItems(addedInIndex + modifiedInIndex + deletedFromIndex);
    ui->addedView->setModel(addedModel);
    changedModel = new GitFileStatusModel(ui->changedView);
    changedModel->setItems(modifiedInWorkingDirectory + deletedFromWorkingDirectory + addedInWorkingDirectory);
    ui->changedView->setModel(changedModel);
    untrackedModel = new GitFileStatusModel(ui->untrackedView);
    untrackedModel->setItems(untrackedFiles);
    ui->untrackedView->setModel(untrackedModel);

    ui->changedView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->changedView->setColumnWidth(1, 1);
    ui->changedView->setColumnWidth(2, 1);
    ui->changedView->setColumnWidth(3, 4);
    ui->addedView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->addedView->setColumnWidth(1, 1);
    ui->addedView->setColumnWidth(2, 1);
    ui->addedView->setColumnWidth(3, 4);
    ui->untrackedView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->untrackedView->setColumnWidth(1, 1);
    ui->untrackedView->setColumnWidth(2, 1);
    ui->untrackedView->setColumnWidth(3, 4);
}

QList<GitFileStatus> GitWidget::getFilesStatus(const QRegularExpression &regex, const QString &results, const QString &status) const
{
    QList<GitFileStatus> ret;
    QRegularExpressionMatchIterator iterator = regex.globalMatch(results);
    while(iterator.hasNext())
    {
        auto match = iterator.next();
        for(int i=1;i<match.capturedLength(); i++)
        {
            auto tmp = match.captured(i);
            if(!tmp.isEmpty())
            {
                const QString name = QFileInfo(tmp).fileName();
                ret.append(GitFileStatus(name.isEmpty() ? tmp : name, tmp, status, "0", "0"));
            }
        }
    }
    return ret;
}

QList<QPair<QString, QPair<QString, QString>>> GitWidget::readDiff()
{
    QProcess* process = new QProcess(this);
    process->setWorkingDirectory(repoPath);
    process->startCommand("git diff --numstat");
    process->waitForStarted();
    process->waitForFinished();
    process->waitForReadyRead();

    QList<QPair<QString, QPair<QString, QString>>> changed;

    const QString results = process->readAllStandardOutput();
    QRegularExpressionMatchIterator iterator = QRegularExpression(R"((\d+)\t(\d+)\t(.+))").globalMatch(results);
    while(iterator.hasNext())
    {
        auto match = iterator.next();
        const QString added = match.captured(1);
        const QString removed = match.captured(2);
        const QString name = match.captured(3);
        if(std::find_if(changed.begin(), changed.end(), [&name](const auto& item){return item.first == name;}) == changed.end())
        {
            changed.append(QPair(name, QPair(added, removed)));
        }
    }
    return changed;
}

void GitWidget::applyDiff(QList<GitFileStatus>& files, QList<QPair<QString, QPair<QString, QString>>>& diffs)
{
    for(auto& i : files)
    {
        const auto tmp = std::find_if(diffs.begin(), diffs.end(), [&i](const auto& item){return i.path == item.first;});
        if(tmp != diffs.end())
        {
            i.addedLines = tmp->second.first;
            i.removedLines = tmp->second.second;
        }
    }
}

void GitWidget::refresh()
{
    readStatus();
}

void GitWidget::gitAdd(const QModelIndex& index)
{
    QProcess* process = new QProcess(this);
    process->setWorkingDirectory(repoPath);
    process->start("git", {"add", changedModel->getItems().at(index.row()).path});
    process->waitForStarted();
    process->waitForFinished();
}

void GitWidget::gitAddUntracked(const QModelIndex &index)
{
    QProcess* process = new QProcess(this);
    process->setWorkingDirectory(repoPath);
    process->start("git", {"add", untrackedModel->getItems().at(index.row()).path});
    process->waitForStarted();
    process->waitForFinished();
}

void GitWidget::gitDiff(const QModelIndex &index)
{
    QProcess* process = new QProcess(this);
    process->setWorkingDirectory(repoPath);
    process->start("git", {"diff", changedModel->getItems().at(index.row()).path});
    process->waitForStarted();
    process->waitForFinished();
    process->waitForReadyRead();
    emit openInEditor(process->readAllStandardOutput(), tr("git diff %1").arg(changedModel->getItems().at(index.row()).path));
}

void GitWidget::gitDiffAdded(const QModelIndex &index)
{
    QProcess* process = new QProcess(this);
    process->setWorkingDirectory(repoPath);
    process->start("git", {"diff", addedModel->getItems().at(index.row()).path});
    process->waitForStarted();
    process->waitForFinished();
    process->waitForReadyRead();
    emit openInEditor(process->readAllStandardOutput(), tr("git diff %1").arg(addedModel->getItems().at(index.row()).path));
}

void GitWidget::openAdded(const QModelIndex &index)
{
    emit openFile(repoPath + QDir::separator() + addedModel->getItems().at(index.row()).path);
}

void GitWidget::openChanged(const QModelIndex &index)
{
    emit openFile(repoPath + QDir::separator() + changedModel->getItems().at(index.row()).path);
}

void GitWidget::openUntracked(const QModelIndex &index)
{
    emit openFile(repoPath + QDir::separator() + untrackedModel->getItems().at(index.row()).path);
}
