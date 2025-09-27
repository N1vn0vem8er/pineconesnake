#include "mainwindow.h"
#include "savewarningdialog.h"
#include "texteditor.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QInputDialog>
#include <QFontDialog>

#define VERSION "1.0.3"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("pcstxteditor");
    restoreState(settings.value("state").toByteArray());
    restoreGeometry(settings.value("geometry").toByteArray());
    defaultFontSize = settings.value("defaultFontSize", 10).toInt();
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
    connect(ui->actionIncrease_Font_Size, &QAction::triggered, this, &MainWindow::increaseFontSize);
    connect(ui->actionDecrease_Font_Size, &QAction::triggered, this, &MainWindow::decreaseFontSize);
    connect(ui->actionSet_Font_Size, &QAction::triggered, this, &MainWindow::setFontSize);
    connect(ui->actionReset_Font_Size, &QAction::triggered, this, &MainWindow::resetFontSize);
    connect(ui->actionSet_Font, &QAction::triggered, this, &MainWindow::setFont);

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
    QSettings settings("pcstxteditor");
    settings.setValue("state", saveState());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("defaultFontSize", defaultFontSize);
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
    TextEditor* editor = new TextEditor(ui->tabWidget);
    editor->setFontSize(defaultFontSize);
    connect(editor, &TextEditor::fontSizeChanged, this, &MainWindow::fontSizeChanged);
    editor->setName(tr("New File"));
    addTab(editor, tr("New File"));
}

void MainWindow::closeTab(const int index)
{
    QWidget* widget = ui->tabWidget->widget(index);
    if(widget)
    {
        TextEditor* editor = qobject_cast<TextEditor*>(widget);
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
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->widget(index));
    if(editor)
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
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
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
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
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
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        editor->copy();
    }
}

void MainWindow::selectAll()
{
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        editor->selectAll();
    }
}

void MainWindow::cut()
{
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        editor->cut();
    }
}

void MainWindow::paste()
{
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        editor->paste();
    }
}

void MainWindow::undo()
{
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        editor->undo();
    }
}

void MainWindow::redo()
{
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        editor->redo();
    }
}

void MainWindow::showSearch()
{
    ui->searchWidget->setVisible(!ui->searchWidget->isVisible());
    for(int i = 0;i < ui->tabWidget->count(); i++)
    {
        TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->widget(i));
        if(editor)
        {
            editor->clearSearchFormatting();
        }
    }
}

void MainWindow::find(const QString &text)
{
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        editor->find(text);
    }
}

void MainWindow::replace(const QString &find, const QString &replace)
{
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        editor->replace(find, replace);
    }
}

void MainWindow::fontSizeChanged(int size)
{
    defaultFontSize = size;
    ui->statusbar->showMessage(tr("Font size: %1").arg(size), 3000);
}

void MainWindow::increaseFontSize()
{
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->increaseFontSize();
}

void MainWindow::decreaseFontSize()
{
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->decreaseFontSize();
}

void MainWindow::setFontSize()
{
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        bool ok;
        int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), editor->font().pointSize(), 1, 255, 1, &ok);
        if(ok)
            editor->setFontSize(size);
    }
}

void MainWindow::setFont()
{
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        bool ok;
        const QFont font = QFontDialog::getFont(&ok, editor->font(), this);
        if(ok)
        {
            defaultFontSize = font.pointSize();
            editor->setFont(font);
        }
    }
}

void MainWindow::resetFontSize()
{
    TextEditor* editor = qobject_cast<TextEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->setFontSize(10);
}

void MainWindow::openFile(const QString& path)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        TextEditor* editor = new TextEditor(ui->tabWidget);
        connect(editor, &TextEditor::fontSizeChanged, this, &MainWindow::fontSizeChanged);
        editor->setFontSize(defaultFontSize);
        editor->setPlainText(file.readAll());
        file.close();
        editor->setPath(path);
        addTab(editor, QFileInfo(file).fileName());
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
        pathLabel->setText(path);
    }
}

void MainWindow::saveFile(const QString &path, const QString &text)
{
    QFile file(path);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
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
