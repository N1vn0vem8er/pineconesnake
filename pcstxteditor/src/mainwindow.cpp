#include "mainwindow.h"
#include "savewarningdialog.h"
#include "texteditor.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#include <QDir>
#include <QMessageBox>

#define VERSION "1.0.3"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::openFileDialog);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);
    connect(ui->actionNew_File, &QAction::triggered, this, &MainWindow::openNewFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionClose_File, &QAction::triggered, this, [this]{closeTab(ui->tabWidget->currentIndex());});
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [this]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_PCS_TxtEditor, &QAction::triggered, this, &MainWindow::displayAboutDialog);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::paste);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui->actionSelect_All, &QAction::triggered, this, &MainWindow::selectAll);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionFind_Replace, &QAction::triggered, this, &MainWindow::showSearch);
    connect(ui->searchWidget, static_cast<void (SearchWidget::*)(const QString&)>(&SearchWidget::find), this, &MainWindow::find);
    connect(ui->searchWidget, static_cast<void (SearchWidget::*)(const QString&, const QString&)>(&SearchWidget::replace), this, &MainWindow::replace);
    connect(ui->action2, &QAction::triggered, this, [this]{setFontSize(2);});
    connect(ui->action4, &QAction::triggered, this, [this]{setFontSize(4);});
    connect(ui->action6, &QAction::triggered, this, [this]{setFontSize(6);});
    connect(ui->action8, &QAction::triggered, this, [this]{setFontSize(8);});
    connect(ui->action10, &QAction::triggered, this, [this]{setFontSize(10);});
    connect(ui->action11, &QAction::triggered, this, [this]{setFontSize(11);});
    connect(ui->action12, &QAction::triggered, this, [this]{setFontSize(12);});
    connect(ui->action13, &QAction::triggered, this, [this]{setFontSize(13);});
    connect(ui->action14, &QAction::triggered, this, [this]{setFontSize(14);});
    connect(ui->action16, &QAction::triggered, this, [this]{setFontSize(16);});
    connect(ui->action18, &QAction::triggered, this, [this]{setFontSize(18);});
    connect(ui->action20, &QAction::triggered, this, [this]{setFontSize(20);});
    connect(ui->action22, &QAction::triggered, this, [this]{setFontSize(22);});
    connect(ui->action24, &QAction::triggered, this, [this]{setFontSize(24);});
    connect(ui->action26, &QAction::triggered, this, [this]{setFontSize(26);});
    connect(ui->action28, &QAction::triggered, this, [this]{setFontSize(28);});
    connect(ui->action30, &QAction::triggered, this, [this]{setFontSize(30);});
    connect(ui->action34, &QAction::triggered, this, [this]{setFontSize(34);});
    connect(ui->action38, &QAction::triggered, this, [this]{setFontSize(38);});
    connect(ui->action42, &QAction::triggered, this, [this]{setFontSize(42);});
    connect(ui->action48, &QAction::triggered, this, [this]{setFontSize(48);});
    connect(ui->action50, &QAction::triggered, this, [this]{setFontSize(50);});
    connect(ui->action60, &QAction::triggered, this, [this]{setFontSize(60);});
    connect(ui->action70, &QAction::triggered, this, [this]{setFontSize(70);});
    connect(ui->action72, &QAction::triggered, this, [this]{setFontSize(72);});


    pathLabel = new QLabel(ui->statusbar);
    ui->statusbar->addWidget(pathLabel);
    ui->searchWidget->setVisible(false);

    QStringList args = qApp->arguments();
    if(args.size() >= 2)
    {
        for(int i=1;i<args.size(); i++)
        {
            openFile(args[i]);
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTab(QWidget *widget, const QString &name)
{
    ui->tabWidget->addTab(widget, name);
}

void MainWindow::openFileDialog()
{
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath());
    if(!filePath.isEmpty())
        openFile(filePath);
}

void MainWindow::openNewFile()
{
    TextEditor* editor = new TextEditor(this);
    editor->setName(tr("New File"));
    addTab(editor, tr("New File"));
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

void MainWindow::displayAboutDialog()
{
    QMessageBox::about(this, tr("About TxtEditor"), QString(tr("<html><body><h3>About TxtEditor</h3><p>PCS TxtEditor is a basic text files editor. It is a part of Pinecone Snake project.</p><p>Version: %1</p><p>License: <a href=\"%2\">GPL 3</a></p></body></html>").arg(VERSION).arg(LICENSELINK).toStdString().c_str()));
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

void MainWindow::setFontSize(int size)
{
    TextEditor* editor = dynamic_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->setFont(QFont(editor->font().family(), size));
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

void MainWindow::openFile(const QString& path)
{
    if(QFileInfo(path).exists())
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

void MainWindow::openSaveWarningDialog(const QString path)
{
    SaveWarningDialog* dialog = new SaveWarningDialog(path, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    if(dialog->exec() == QDialog::Accepted)
    {
        save();
    }
}
