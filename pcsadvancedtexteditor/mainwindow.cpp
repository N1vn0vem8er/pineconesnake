#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "savewarningdialog.h"
#include "texteditor.h"
#include "globals.h"
#include <qfileinfo.h>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newFilePressed);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);

    ui->searchWidget->setVisible(false);
    ui->stackedWidget->setVisible(false);
    pathLabel = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(pathLabel);
    loadHunspell();
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
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath());
    if(!filePath.isEmpty())
        openFile(filePath);
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
            addTab(new TextEditor(content, path), QFileInfo(file).fileName());
            ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
            pathLabel->setText(path);
            file.close();
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
            ui->menuLanguages->addAction(action);
        }
    }
}

void MainWindow::openDir(const QString &path)
{

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
    }
    else
    {
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
    addTab(editor, tr("New File"));
}

void MainWindow::openFilePressed()
{

}

void MainWindow::openDirPressed()
{

}
