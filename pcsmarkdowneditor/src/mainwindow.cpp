#include "mainwindow.h"
#include "codeeditor.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("pcsmarkdowneditor");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    ui->actionFiles->setChecked(settings.value("filesVisible", false).toBool());
    ui->treeView->setVisible(ui->actionFiles->isChecked());
    ui->actionMarkdown_Preview->setChecked(settings.value("markdownPreviewVisible", true).toBool());
    ui->webEngineView->setVisible(ui->actionMarkdown_Preview->isChecked());

    ui->splitter->setStretchFactor(1, 2);

    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionOpen_Folder, &QAction::triggered, this, &MainWindow::openDir);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::redo);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::paste);
    connect(ui->actionSelect_All, &QAction::triggered, this, &MainWindow::selectAll);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::deleteText);
    connect(ui->actionDelete_All, &QAction::triggered, this, &MainWindow::deleteAllText);
    connect(ui->actionClose, &QAction::triggered, this, [&]{closeTab(ui->tabWidget->currentIndex());});
    connect(ui->actionClose_All, &QAction::triggered, this, [&]{while(ui->tabWidget->count() > 0) closeTab(ui->tabWidget->currentIndex());});
    connect(ui->actionClose_All_But_This, &QAction::triggered, this, [&]{
        for(int i=ui->tabWidget->count()-1; i >= 0;--i)
        {
            if(i != ui->tabWidget->currentIndex())
                closeTab(i);
        }});
    connect(ui->actionFiles, &QAction::triggered, ui->treeView, &FileSystemTree::setVisible);
    connect(ui->actionMarkdown_Preview, &QAction::triggered, ui->webEngineView, &QWidget::setVisible);
}

MainWindow::~MainWindow()
{
    QSettings settings("pcsmarkdowneditor");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.setValue("filesVisible", ui->actionFiles->isChecked());
    settings.setValue("markdownPreviewVisible", ui->actionMarkdown_Preview->isChecked());
    delete ui;
}

CodeEditor *MainWindow::createEditor(const QString &path, const QString &content)
{
    CodeEditor* editor = new CodeEditor(ui->tabWidget);
    editor->setPath(path);
    editor->setPlainText(content);
    return editor;
}

void MainWindow::newFile()
{
    ui->tabWidget->addTab(createEditor("", ""), tr("New File"));
}

void MainWindow::openFile()
{
    const QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open"), QDir::homePath(), tr("*.md"));
    for(const QString& path : paths)
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->tabWidget->addTab(createEditor(path, file.readAll()), file.fileName());
            file.close();
        }
    }
}

void MainWindow::openDir()
{
    const QString path = QFileDialog::getExistingDirectory(this, tr("Open Dir"), QDir::homePath());
    if(!path.isEmpty())
    {
        ui->treeView->open(path);
    }
}

void MainWindow::save()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        if(!editor->getPath().isEmpty())
        {
            QFile file(editor->getPath());
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(editor->toPlainText().toUtf8());
                file.close();
            }
        }
        else
            saveAs();
    }

}

void MainWindow::saveAs()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), editor->isEnabled() ? QDir::homePath() : QFileInfo(editor->getPath()).absoluteDir().absolutePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(editor->toPlainText().toUtf8());
                file.close();
                editor->setPath(path);
            }
        }
    }
}

void MainWindow::closeTab(int index)
{
    QWidget* widget = ui->tabWidget->widget(index);
    CodeEditor* editor = qobject_cast<CodeEditor*>(widget);
    if(editor)
    {
        if(!editor->isSaved())
        {
            if(QMessageBox::question(this, tr("Save"), tr("Save file %1?").arg(editor->getName())))
                save();
        }
    }
    ui->tabWidget->removeTab(index);
    widget->deleteLater();
}

void MainWindow::undo()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->undo();
}

void MainWindow::redo()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->redo();
}

void MainWindow::cut()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->cut();
}

void MainWindow::copy()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->copy();
}

void MainWindow::paste()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->paste();
}

void MainWindow::deleteText()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->deleteSelected();
}

void MainWindow::deleteAllText()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->deleteAll();
}

void MainWindow::selectAll()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->selectAll();
}
