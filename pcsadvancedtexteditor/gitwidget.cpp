#include "gitwidget.h"
#include "ui_gitwidget.h"
#include <qprocess.h>

GitWidget::GitWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GitWidget)
{
    ui->setupUi(this);
    setVisibility(false);
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
    ui->addedListView->setVisible(val);
    ui->changedListView->setVisible(val);
    ui->untrackedListView->setVisible(val);
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
    untrackedFiles = getFilesStatus(QRegularExpression(R"(\?\?\s+(.*))"), results);
    modifiedInBoth = getFilesStatus(QRegularExpression(R"(MM\s+(.*))"), results);
    modifiedInIndex = getFilesStatus(QRegularExpression(R"(M.\s+(.*))"), results);
    modifiedInWorkingDirectory = getFilesStatus(QRegularExpression(R"(.M\s+(.*))"), results);
    addedInIndex = getFilesStatus(QRegularExpression(R"(A.\s+(.*))"), results);
    deletedFromIndex = getFilesStatus(QRegularExpression(R"(D.\s+(.*))"), results);
}

QStringList GitWidget::getFilesStatus(const QRegularExpression &regex, const QString &results)
{
    QStringList ret;
    QRegularExpressionMatchIterator iterator = regex.globalMatch(results);
    while(iterator.hasNext())
    {
        auto match = iterator.next();
        for(int i=1;i<match.capturedLength(); i++)
        {
            if(!match.captured(i).isEmpty())
                ret.append(match.captured(i));
        }
    }
    return ret;
}
