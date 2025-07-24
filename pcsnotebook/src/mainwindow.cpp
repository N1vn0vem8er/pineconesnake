#include "mainwindow.h"
#include "inputtitledialog.h"
#include "qdatetime.h"
#include "qmessagebox.h"
#include "resourcesmanager.h"
#include "ui_mainwindow.h"
#include "writer.h"
#include "allnoteswidget.h"

#define VERSION "1.0.0"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->newButton, &QPushButton::clicked, this, &MainWindow::addNewNote);
    connect(ui->allButton, &QPushButton::clicked, this, &MainWindow::showAllNotes);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, [&](int index){delete ui->tabWidget->widget(index);});
    connect(ui->actionClose_Tab, &QAction::triggered, this, [&]{delete ui->tabWidget->currentWidget();});
    connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::refreshNotes);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::paste);
    connect(ui->actionSelectAll, &QAction::triggered, this, &MainWindow::selectAll);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::addNewNote);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionAbout_Notebook, &QAction::triggered, this, &MainWindow::displayAbout);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [this]{QMessageBox::aboutQt(this, tr("About Qt"));});
    showAllNotes();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTab(const QString &title, QWidget *widget)
{
    ui->tabWidget->addTab(widget, title);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void MainWindow::addNewNote()
{
    InputTitleDialog dialog;
    if(dialog.exec() == QDialog::Accepted)
    {
        const QString title = dialog.getTitle();
        QDateTime dt = QDateTime::currentDateTime();
        Note note = Note(-1, title, "", dt.toString("yyyy-MM-dd HH:mm:ss"), dt.toString("yyyy-MM-dd HH:mm:ss"));
        ResourcesManager* rm = ResourcesManager::getInstance();
        rm->addNote(note);
        refreshNotes();
        note.id = rm->getLastId();
        openWriter(note);
    }
}

void MainWindow::showAllNotes()
{
    AllNotesWidget* widget = new AllNotesWidget(this);
    connect(widget, &AllNotesWidget::openWriter, this, &MainWindow::openWriter);
    addTab("Notes", widget);
}

void MainWindow::openWriter(const Note &note)
{
    Writer* widget = new Writer(note, this);
    connect(widget, &Writer::requestDelete, this, &MainWindow::deleteRequested);
    connect(widget, &Writer::changed, this, &MainWindow::refreshNotes);
    addTab(note.title, widget);
}

void MainWindow::refreshNotes()
{
    for(int i = 0; i< ui->tabWidget->count(); i++)
    {
        AllNotesWidget* widget = dynamic_cast<AllNotesWidget*>(ui->tabWidget->widget(i));
        if(widget)
        {
            widget->all();
        }
    }
}

void MainWindow::save()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget)
    {
        widget->manualSave();
    }
}

void MainWindow::copy()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget)
    {
        widget->copy();
    }
}

void MainWindow::cut()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget)
    {
        widget->cut();
    }
}

void MainWindow::paste()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget)
    {
        widget->paste();
    }
}

void MainWindow::selectAll()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget)
    {
        widget->selectAll();
    }
}

void MainWindow::saveAs()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget)
    {
        widget->saveAs();
    }
}

void MainWindow::undo()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget)
    {
        widget->undo();
    }
}

void MainWindow::redo()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget)
    {
        widget->redo();
    }
}

void MainWindow::deleteRequested(const Note& note)
{
    QMessageBox::StandardButton dialog = QMessageBox::question(this, tr("Delete note?"), tr("Do you want to delete %1?").arg(note.title), QMessageBox::Yes | QMessageBox::No);
    if(dialog == QMessageBox::Yes)
    {
        ResourcesManager::getInstance()->deleteNote(note);
        delete ui->tabWidget->currentWidget();
    }
    for(int i=0;i<ui->tabWidget->count(); i++)
    {
        AllNotesWidget* widget = dynamic_cast<AllNotesWidget*>(ui->tabWidget->currentWidget());
        if(widget)
        {
            widget->all();
        }
    }
}

void MainWindow::displayAbout()
{
    QMessageBox::about(this, tr("About Notebook"), tr(R"(
    <html>
        <body>
            <h3>PCS Notebook</h3>
            <p>PCS Notebook is an application for taking notes. It is a part of Pinecone Snake project.</p>
            <p>Version: %1</p>
            <p>License: <a href=\"%2\">GPL 3</a></p>
        </body>
    </html>)").arg(VERSION, LICENSELINK));
}
