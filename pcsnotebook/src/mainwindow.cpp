#include "mainwindow.h"
#include "inputtitledialog.h"
#include "qdatetime.h"
#include "resourcesmanager.h"
#include "ui_mainwindow.h"
#include "writer.h"
#include "allnoteswidget.h"

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
        addTab(title, new Writer(note, this));
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
    addTab(note.title, new Writer(note, this));
}

void MainWindow::refreshNotes()
{
    AllNotesWidget* widget = dynamic_cast<AllNotesWidget*>(ui->tabWidget->currentWidget());
    if(widget!= nullptr)
    {
        widget->all();
    }
}

void MainWindow::save()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget!= nullptr)
    {
        widget->manualSave();
    }
}

void MainWindow::copy()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget!= nullptr)
    {
        widget->copy();
    }
}

void MainWindow::cut()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget!= nullptr)
    {
        widget->cut();
    }
}

void MainWindow::paste()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget!= nullptr)
    {
        widget->paste();
    }
}

void MainWindow::selectAll()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget!= nullptr)
    {
        widget->selectAll();
    }
}

void MainWindow::saveAs()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget!= nullptr)
    {
        widget->saveAs();
    }
}

void MainWindow::undo()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget!= nullptr)
    {
        widget->undo();
    }
}

void MainWindow::redo()
{
    Writer* widget = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(widget!= nullptr)
    {
        widget->redo();
    }
}
