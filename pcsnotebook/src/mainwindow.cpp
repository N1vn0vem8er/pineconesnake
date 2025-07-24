#include "mainwindow.h"
#include "inputtitledialog.h"
#include "qdatetime.h"
#include "qimage.h"
#include "qmessagebox.h"
#include "resourcesmanager.h"
#include "ui_mainwindow.h"
#include "writer.h"
#include "allnoteswidget.h"
#include <QFileDialog>
#include <QLineEdit>

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
    connect(ui->actionMerge_lines, &QAction::triggered, this, &MainWindow::mergeLines);
    connect(ui->actionincreaseFontSize, &QAction::triggered, this, &MainWindow::increaseFontSize);
    connect(ui->actionDecrease_font_size, &QAction::triggered, this, &MainWindow::decreaseFontSize);
    connect(ui->actionReset_font_size, &QAction::triggered, this, &MainWindow::resetFontSize);
    connect(ui->actionSet_font_size, &QAction::triggered, this, &MainWindow::setFontSize);
    connect(ui->actionMAKE_SELECTED_CAPITAL, &QAction::triggered, this, &MainWindow::makeCapitalEveryWord);
    connect(ui->actionmake_selected_small, &QAction::triggered, this, &MainWindow::makeSmallLetters);

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
void MainWindow::mergeLines()
{
    Writer* editor = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->mergeSelectedLines();
    }
}

void MainWindow::makeSmallLetters()
{
    Writer* editor = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->makeSelectedSmall();
    }
}

void MainWindow::makeCapitalLetters()
{
    Writer* editor = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->makeSelectedCapital();
    }
}

void MainWindow::makeCapitalSentences()
{
    Writer* editor = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->makeSelectedSentenceCapital();
    }
}

void MainWindow::makeCapitalEveryWord()
{
    Writer* editor = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->makeEverySelectedCapital();
    }
}

void MainWindow::openPasteFromFile()
{
    Writer* editor = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
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
                    editor->appendText(file.readAll());
                    file.close();
                }
            }
        }
    }
}

void MainWindow::setLineWrap(bool val)
{
    Writer* editor = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->setLineWrap(val);
    }
}

void MainWindow::increaseFontSize()
{
    Writer* editor = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->increaseFontSize();
    }
}

void MainWindow::decreaseFontSize()
{
    Writer* editor = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->decreaseFontSize();
    }
}

void MainWindow::resetFontSize()
{
    Writer* editor = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
    if(editor != nullptr)
    {
        editor->setFontSize(10);
    }
}

void MainWindow::setFontSize()
{
    Writer* editor = dynamic_cast<Writer*>(ui->tabWidget->currentWidget());
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
