#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mimefinderworker.h"
#include "savewarningdialog.h"
#include "settings.h"
#include "texteditor.h"
#include "globals.h"
#include "startwidget.h"
#include <qfileinfo.h>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <qmessagebox.h>
#include <qmimedata.h>
#include <qprocess.h>
#include <qpushbutton.h>
#include <qthread.h>
#include <QPrinter>
#include <QPrintDialog>
#include <QLineEdit>

#define VERSION "0.0.1"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"
#define PROJECTLINK "https://github.com/N1vn0vem8er/pineconesnake"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    startGettingMimeData();
    setAcceptDrops(true);

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newFilePressed);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionClose, &QAction::triggered, this, [&]{closeTab(ui->tabWidget->currentIndex());});
    connect(ui->actionClose_All, &QAction::triggered, this, [&]{while(ui->tabWidget->count())closeTab(ui->tabWidget->currentIndex());});
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::paste);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui->actionSelect_All, &QAction::triggered, this, &MainWindow::selectAll);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->searchWidget, static_cast<void (SearchWidget::*)(const QString&)>(&SearchWidget::find), this, &MainWindow::find);
    connect(ui->searchWidget, static_cast<void (SearchWidget::*)(const QString&, const QString&)>(&SearchWidget::replace), this, &MainWindow::replace);
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::redo);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::deleteSelected);
    connect(ui->actionDelete_All, &QAction::triggered, this, &MainWindow::deleteAll);
    connect(ui->actionEnabled, &QAction::triggered, this, &MainWindow::spellCheckSwitch);
    connect(ui->actionFiles, &QAction::triggered, this, [&]{ui->stackedWidget->setVisible(ui->stackedWidget->currentIndex() == 0 && ui->stackedWidget->isVisible() ? false : true); ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->actionGit, &QAction::triggered, this, [&]{ui->stackedWidget->setVisible(ui->stackedWidget->currentIndex() == 1 && ui->stackedWidget->isVisible() ? false : true); ui->stackedWidget->setCurrentIndex(1);});
    connect(ui->actionFindReplace, &QAction::triggered, this, &MainWindow::showSearch);
    connect(ui->actionOpen_Directory, &QAction::triggered, this, &MainWindow::openDirPressed);
    connect(ui->filesPage, &FileSystemTree::openFile, this, &MainWindow::openFile);
    connect(ui->gitPage, &GitWidget::openInEditor, this, &MainWindow::openWithText);
    connect(ui->gitPage, &GitWidget::openFile, this, &MainWindow::openFile);
    connect(ui->actionStart, &QAction::triggered, this, &MainWindow::showStart);
    connect(ui->gitPage, &GitWidget::addTab, this, &MainWindow::addTab);
    connect(ui->actionClose_All_But_This, &QAction::triggered, this, &MainWindow::closeAllButThis);
    connect(ui->actionReload, &QAction::triggered, this, &MainWindow::reloadCurrent);
    connect(ui->actionReload_All, &QAction::triggered, this, &MainWindow::reloadAll);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::openPrint);
    connect(ui->actionOverwriteMode, &QAction::triggered, this, &MainWindow::overwriteModeChanged);
    connect(ui->actionRead_Only, &QAction::triggered, this, &MainWindow::readOnlyChanget);
    connect(ui->actionMerge_Lines, &QAction::triggered, this, &MainWindow::mergeLines);
    connect(ui->actionSmall_Letters, &QAction::triggered, this, &MainWindow::makeSmallLetters);
    connect(ui->actionLarge_Letters, &QAction::triggered, this, &MainWindow::makeCapitalLetters);
    connect(ui->actionCapital_Letter_every_word, &QAction::triggered, this, &MainWindow::makeCapitalEveryWord);
    connect(ui->actionCapital_Letter_at_Sentence, &QAction::triggered, this, &MainWindow::makeCapitalSentences);
    connect(ui->actionPaste_From_File, &QAction::triggered, this, &MainWindow::openPasteFromFile);
    connect(ui->actionLine_Wrap, &QAction::triggered, this, &MainWindow::setLineWrap);
    connect(ui->actionIncrease_Font_Size, &QAction::triggered, this, &MainWindow::increaseFontSize);
    connect(ui->actionDecrease_Font_Size, &QAction::triggered, this, &MainWindow::decreaseFontSize);
    connect(ui->actionReset_Font_Size, &QAction::triggered, this, &MainWindow::resetFontSize);
    connect(ui->actionSet_Font_Size, &QAction::triggered, this, &MainWindow::setFontSize);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [this]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_Application, &QAction::triggered, this, &MainWindow::openAbout);

    ui->searchWidget->setVisible(false);
    ui->stackedWidget->setVisible(false);
    ui->splitter->setStretchFactor(1, 1);
    pathLabel = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(pathLabel);
    languageLabel = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(languageLabel);
    loadHunspell();
    ui->filesPage->open(QDir::homePath());

    showStart();
    loadRecentFiles();
    loadRecentDirs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveAs()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        const QString path = QFileDialog::getSaveFileName(this, tr("Save File As"), QDir::homePath());
        if(!path.isEmpty())
        {
            saveFile(path, editor->toPlainText());
            editor->setSaved(true);
            saveFileToRecent(path);
        }
    }
}

void MainWindow::save()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        if(editor->getPath().isEmpty())
        {
            saveAs();
        }
        else
        {
            saveFile(editor->getPath(), editor->toPlainText());
            editor->setSaved(true);
        }
    }
}

void MainWindow::saveFile(const QString &path, const QString &text)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        file.write(text.toLatin1());
        ui->statusbar->showMessage(tr("Saved %1").arg(path), 5000);
        file.close();
    }
    else
        ui->statusbar->showMessage(tr("Couldn't save %1").arg(path), 5000);
}

void MainWindow::open()
{
    const QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("Open File"), QDir::homePath());
    if(!filePaths.isEmpty())
        for(const auto& filePath : filePaths)
            openFile(filePath);
}

void MainWindow::openWithText(const QString &text, const QString& title, bool readOnly, bool spellChecking)
{
    TextEditor* editor = new TextEditor(ui->tabWidget);
    editor->setPlainText(text);
    editor->setReadOnly(readOnly);
    editor->setSpellCheckEnabled(spellChecking);
    connect(editor, &TextEditor::fontSizeChanged, this, &MainWindow::fontSizeChanged);
    ui->actionRead_Only->setChecked(readOnly);
    addTab(editor, title);
}

void MainWindow::openFile(const QString& path)
{
    if(QFileInfo::exists(path))
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        if(file.isOpen())
        {
            QString content = file.readAll();
            TextEditor* editor = new TextEditor(content, path);
            connect(editor, &TextEditor::fontSizeChanged, this, &MainWindow::fontSizeChanged);
            addTab(editor, QFileInfo(file).fileName());
            ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
            pathLabel->setText(path);
            saveFileToRecent(path);
            loadRecentFiles();
            file.close();
            emit refreshStartWidgets();
        }
    }
}

void MainWindow::loadHunspell()
{
    QDir dir("/usr/share/hunspell");
    if(dir.exists())
    {
        Globals::hunspellLanguages = dir.entryList(QStringList() << "*.dic" << "*.aff", QDir::Files);
        for(int i=0; i<Globals::hunspellLanguages.size(); i+=2)
        {
            QAction* action = new QAction(Globals::hunspellLanguages[i].left(Globals::hunspellLanguages[i].indexOf(".")), ui->menuLanguages);
            connect(action, &QAction::triggered, this, [this, action]{changeLanguageForEditor(action->text());});
            ui->menuLanguages->addAction(action);
        }
        languageLabel->setText(Settings::defaultLanguage);
    }
}

void MainWindow::startGettingMimeData()
{
    MimeFinderWorker* worker = new MimeFinderWorker();
    mimeFinderThread = new QThread(this);
    worker->moveToThread(mimeFinderThread);
    connect(mimeFinderThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::startMimeSearch, worker, &MimeFinderWorker::start);
    connect(worker, &MimeFinderWorker::finished, mimeFinderThread, &QThread::quit);
    connect(mimeFinderThread, &QThread::finished, mimeFinderThread, &QThread::deleteLater);
    mimeFinderThread->start();
    emit startMimeSearch();
}

void MainWindow::saveFileToRecent(const QString &path)
{
    QFile file(Settings::recentFilesStoragePath);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    if(file.isOpen())
    {
        QStringList paths;
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            paths << stream.readLine();
        }
        if(!paths.contains(path))
        {
            file.seek(0);
            if(file.isOpen())
            {
                QTextStream stream(&file);
                if(paths.length() > 10) paths.removeAt(0);
                paths.append(path);
                for(const auto& i : std::as_const(paths))
                {
                    stream << i + "\n";
                }
            }
        }
        file.close();
    }
}

void MainWindow::saveDirToRecent(const QString &path)
{
    QFile file(Settings::recentDirsStoragePath);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    if(file.isOpen())
    {
        QStringList paths;
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            paths << stream.readLine();
        }
        if(!paths.contains(path))
        {
            file.seek(0);
            if(file.isOpen())
            {
                QTextStream stream(&file);
                if(paths.length() > 10) paths.removeAt(0);
                paths.append(path);
                for(const auto& i : std::as_const(paths))
                {
                    stream << i + "\n";
                }
            }
        }
        file.close();
    }
}

void MainWindow::loadRecentFiles()
{
    ui->menuRecent_Files->clear();
    if(!QDir(Settings::storagePath).exists())
    {
        QDir().mkpath(Settings::storagePath);
    }
    QFile file(Settings::recentFilesStoragePath);
    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            const QString line = stream.readLine();
            if(!line.isEmpty())
            {
                QAction* action = new QAction(ui->menuRecent_Files);
                action->setText(line);
                connect(action, &QAction::triggered, this, [this, line]{openFile(line);});
                ui->menuRecent_Files->addAction(action);
            }
        }
        file.close();
    }
    if(ui->menuRecent_Files->isEmpty())
    {
        QAction* action = new QAction(tr("no recent"), ui->menuRecent_Files);
        action->setEnabled(false);
        ui->menuRecent_Files->addAction(action);
    }
}

void MainWindow::loadRecentDirs()
{
    ui->menuRecent_Directories->clear();
    if(!QDir(Settings::storagePath).exists())
    {
        QDir().mkpath(Settings::storagePath);
    }
    QFile file(Settings::recentDirsStoragePath);
    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            const QString line = stream.readLine();
            if(!line.isEmpty())
            {
                QAction* action = new QAction(ui->menuRecent_Directories);
                action->setText(line);
                connect(action, &QAction::triggered, this, [this, line]{openDir(line);});
                ui->menuRecent_Directories->addAction(action);
            }
        }
        file.close();
    }
    if(ui->menuRecent_Directories->isEmpty())
    {
        QAction* action = new QAction(tr("no recent"), ui->menuRecent_Directories);
        action->setEnabled(false);
        ui->menuRecent_Directories->addAction(action);
    }
}

void MainWindow::openDir(const QString &path)
{
    if(QFileInfo(path).isDir())
    {
        QProcess* process = new QProcess(this);
        process->setWorkingDirectory(path);
        process->startCommand("git status");
        process->waitForStarted();
        process->waitForFinished();
        process->waitForReadyRead();
        hasGitRepository = process->readAllStandardError().isEmpty();
        ui->filesPage->open(path);
        ui->filesPage->setHasGitRepository(hasGitRepository);
        if(hasGitRepository) ui->gitPage->setRepositoryPath(path);
        else ui->gitPage->noRepo();
        saveDirToRecent(path);
        loadRecentDirs();
        emit refreshStartWidgets();
        ui->stackedWidget->setVisible(ui->stackedWidget->currentIndex() == 0 && ui->stackedWidget->isVisible() ? false : true); ui->stackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::addTab(QWidget *widget, const QString &title)
{
    ui->tabWidget->addTab(widget, title);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void MainWindow::closeTab(const int index)
{
    QWidget* widget = ui->tabWidget->widget(index);
    if(widget != nullptr)
    {
        TextEditor* editor = dynamic_cast<TextEditor*>(widget);
        if(editor!=nullptr && !editor->isSaved())
        {
            openSaveWarningDialog(editor->getName());
        }
        ui->tabWidget->removeTab(index);
        delete widget;
    }
}

void MainWindow::tabChanged(const int index)
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->widget(index));
    if(editor != nullptr)
    {
        pathLabel->setText(editor->getPath());
        ui->actionEnabled->setChecked(editor->getSpellCheckEnabled());
        ui->actionRead_Only->setChecked(editor->isReadOnly());
        ui->actionOverwriteMode->setChecked(editor->overwriteMode());
        ui->actionLine_Wrap->setChecked(editor->lineWrapMode());
    }
    else
    {
        ui->actionEnabled->setChecked(Settings::spellCheckEnabled);
        pathLabel->clear();
    }
}

void MainWindow::openSaveWarningDialog(const QString path)
{
    SaveWarningDialog* dialog = new SaveWarningDialog(path, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    if(dialog->exec() == QDialog::Accepted)
    {
        save();
    }
}

void MainWindow::newFilePressed()
{
    TextEditor* editor = new TextEditor(ui->tabWidget);
    connect(editor, &TextEditor::fontSizeChanged, this, &MainWindow::fontSizeChanged);
    addTab(editor, tr("New File"));
}

void MainWindow::openFilePressed()
{

}

void MainWindow::openDirPressed()
{
    const QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"));
    if(!path.isEmpty())
        openDir(path);
}

void MainWindow::copy()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->copy();
    }
}

void MainWindow::selectAll()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->selectAll();
    }
}

void MainWindow::cut()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->cut();
    }
}

void MainWindow::paste()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->paste();
    }
}

void MainWindow::undo()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->undo();
    }
}

void MainWindow::redo()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->redo();
    }
}

void MainWindow::showSearch()
{
    ui->searchWidget->setVisible(!ui->searchWidget->isVisible());
    for(int i = 0;i < ui->tabWidget->count(); i++)
    {
        TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->widget(i));
        if(editor != nullptr)
        {
            editor->clearSearchFormatting();
        }
    }
}

void MainWindow::find(const QString &text)
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->find(text);
    }
}

void MainWindow::replace(const QString &find, const QString &replace)
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->replace(find, replace);
    }
}

void MainWindow::deleteSelected()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->deleteSelected();
    }
}

void MainWindow::deleteAll()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->deleteAll();
    }
}

void MainWindow::spellCheckSwitch(bool val)
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->setSpellCheckEnabled(val);
    }
    else
    {
        Settings::spellCheckEnabled = val;
    }
}

void MainWindow::changeLanguageForEditor(const QString &language)
{
    languageLabel->setText(language);
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->setLanguage(language);
    }
}

void MainWindow::showStart()
{
    StartWidget* widget = new StartWidget(ui->tabWidget);
    connect(widget, &StartWidget::newFile, this, &MainWindow::newFilePressed);
    connect(widget, &StartWidget::openFile, this, &MainWindow::open);
    connect(widget, &StartWidget::openDir, this, &MainWindow::openDirPressed);
    connect(this, &MainWindow::refreshStartWidgets, widget, &StartWidget::refresh);
    connect(widget, &StartWidget::openFileFromRecent, this, &MainWindow::openFile);
    connect(widget, &StartWidget::openDirFromRecent, this, &MainWindow::openDir);
    connect(widget, &StartWidget::recentFilesCleared, this, &MainWindow::loadRecentFiles);
    connect(widget, &StartWidget::recentDirsCleared, this, &MainWindow::loadRecentDirs);
    addTab(widget, tr("Welcome"));
}

void MainWindow::closeAllButThis()
{
    for(int i=ui->tabWidget->count()-1; i >= 0;--i)
    {
        if(i != ui->tabWidget->currentIndex())
            closeTab(i);
    }
}

void MainWindow::reloadCurrent()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr && !editor->getPath().isEmpty())
    {
        if(!editor->isSaved())
        {
            openSaveWarningDialog(editor->getName());
        }
        QFile file(editor->getPath());
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            editor->setPlainText(file.readAll());
            file.close();
            editor->setSaved(true);
        }
    }
}

void MainWindow::reloadAll()
{
    for(int i=0; i<ui->tabWidget->count(); i++)
    {
        TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->widget(i));
        if(editor != nullptr && !editor->getPath().isEmpty())
        {
            if(!editor->isSaved())
            {
                openSaveWarningDialog(editor->getName());
            }
            QFile file(editor->getPath());
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                editor->setPlainText(file.readAll());
                file.close();
                editor->setSaved(true);
            }
        }
    }
}

void MainWindow::openPrint()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        QPrinter printer(QPrinter::HighResolution);
        QPrintDialog dialog(&printer, this);
        if(dialog.exec() == QDialog::Accepted)
        {
            editor->print(&printer);
        }
    }
}

void MainWindow::overwriteModeChanged(bool val)
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->setOverwriteMode(val);
    }
}

void MainWindow::readOnlyChanget(bool val)
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->setReadOnly(val);
    }
}

void MainWindow::mergeLines()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->mergeSelectedLines();
    }
}

void MainWindow::makeSmallLetters()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->makeSelectedSmall();
    }
}

void MainWindow::makeCapitalLetters()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->makeSelectedCapital();
    }
}

void MainWindow::makeCapitalSentences()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->makeSelectedSentenceCapital();
    }
}

void MainWindow::makeCapitalEveryWord()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->makeEverySelectedCapital();
    }
}

void MainWindow::fontSizeChanged(int size)
{
    ui->statusbar->showMessage(tr("Font size: %1").arg(size), 2000);
}

void MainWindow::openPasteFromFile()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        const QStringList paths = QFileDialog::getOpenFileNames(this, tr("Paste from file"), QDir::homePath());
        if(!paths.isEmpty())
        {
            for(const auto& path : paths)
            {
                QFile file(path);
                file.open(QIODevice::ReadOnly);
                if(file.isOpen())
                {
                    editor->appendPlainText(file.readAll());
                    file.close();
                }
            }
        }
    }
}

void MainWindow::setLineWrap(bool val)
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->setLineWrapMode(val ? QPlainTextEdit::LineWrapMode::WidgetWidth : QPlainTextEdit::LineWrapMode::NoWrap);
    }
}

void MainWindow::increaseFontSize()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->increaseFontSize();
    }
}

void MainWindow::decreaseFontSize()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->decreaseFontSize();
    }
}

void MainWindow::resetFontSize()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->setFontSize(Settings::defaultFontSize);
    }
}

void MainWindow::setFontSize()
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        QDialog* dialog = new QDialog(this);
        QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
        QLineEdit* lineEdit = new QLineEdit(dialog);
        lineEdit->setPlaceholderText(tr("Enter font size"));
        mainLayout->addWidget(lineEdit);
        QPushButton* okButton = new QPushButton(dialog);
        QPushButton* cancelButton = new QPushButton(dialog);
        QHBoxLayout* buttonsLayout = new QHBoxLayout();
        okButton->setText(tr("Ok"));
        cancelButton->setText(tr("Cancel"));
        buttonsLayout->addWidget(okButton);
        buttonsLayout->addWidget(cancelButton);
        mainLayout->addLayout(buttonsLayout);
        connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
        if(dialog->exec() == QDialog::Accepted)
        {
            editor->setFontSize(lineEdit->text().toInt());
        }
        delete buttonsLayout;
    }
}

void MainWindow::openAbout()
{
    QMessageBox::about(this, tr("About PCS Advanced Text Editor"), tr(R"(
    <html>
        <body>
            <h3>PCS Advanced Text Editor</h3>
            <p>PCS Advanced Text Editor is an application for editing text files. It is a feature rich version of <a href="%1">Txt Editor</a>. It is a part of <a href="%2">Pinecone Snake project.</a></p>
            <p>Version: %3</p>
            <p>License: <a href="%4">GPL 3</a></p>
        </body>
    </html>
)").arg("https://github.com/N1vn0vem8er/pineconesnake/blob/main/pcstxteditor/README.md", PROJECTLINK, VERSION, LICENSELINK));
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    if(mimeData->hasUrls())
    {
        QList<QUrl> urls = mimeData->urls();
        for (int i = 0; i < urls.size() && i < 32; i++)
        {
            openFile(urls.at(i).toLocalFile());
        }
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}
