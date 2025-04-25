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
