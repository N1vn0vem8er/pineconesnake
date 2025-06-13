#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "savewarningdialog.h"
#include "texteditor.h"
#include <qfileinfo.h>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newFilePressed);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);

    ui->searchWidget->setVisible(false);
    ui->stackedWidget->setVisible(false);
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
        file.close();
    }
}

void MainWindow::open(const QString &path)
{

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
