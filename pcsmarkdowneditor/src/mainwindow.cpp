#include "mainwindow.h"
#include "codeeditor.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("pcsmarkdowneditor");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());

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
}

MainWindow::~MainWindow()
{
    QSettings settings("pcsmarkdowneditor");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
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

}

void MainWindow::undo()
{

}

void MainWindow::redo()
{

}

void MainWindow::cut()
{

}

void MainWindow::copy()
{

}

void MainWindow::paste()
{

}

void MainWindow::deleteText()
{

}

void MainWindow::deleteAllText()
{

}

void MainWindow::selectAll()
{

}
