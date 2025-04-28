#include "writer.h"
#include "qdatetime.h"
#include "ui_writer.h"
#include "resourcesmanager.h"

#include <QFileDialog>
#include <QTimer>

Writer::Writer(const Note &note, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Writer)
{
    this->note = note;
    ui->setupUi(this);
    ui->editor->setPlainText(this->note.content);
    connect(ui->editor, &TextEditor::textChanged, this, &Writer::textChanged);
    connect(ui->deleteButton, &QPushButton::clicked, this, &Writer::deletePressed);
    connect(ui->copyButton, &QPushButton::clicked, this, &Writer::copy);
    connect(ui->cutButton, &QPushButton::clicked, this, &Writer::cut);
    connect(ui->pasteButton, &QPushButton::clicked, this, &Writer::paste);
    connect(ui->selectAllButton, &QPushButton::clicked, this, &Writer::selectAll);
    connect(ui->saveAsButton, &QPushButton::clicked, this, &Writer::saveAs);
    connect(ui->undoButton, &QPushButton::clicked, this, &Writer::undo);
    connect(ui->redoButton, &QPushButton::clicked, this, &Writer::redo);
    connect(ui->saveButton, &QPushButton::clicked, this, &Writer::manualSave);
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Writer::save);
    timer->setInterval(1000);
    timer->setSingleShot(true);
}

Writer::~Writer()
{
    delete timer;
    delete ui;
}

void Writer::save()
{
    note.content = ui->editor->toPlainText();
    note.modified = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ResourcesManager::getInstance()->editNote(note);
    ui->savedLabel->setVisible(true);
}

void Writer::copy()
{
    ui->editor->copy();
}

void Writer::cut()
{
    ui->editor->cut();
}

void Writer::paste()
{
    ui->editor->paste();
}

void Writer::selectAll()
{
    ui->editor->selectAll();
}

void Writer::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::WriteOnly);
        if(file.isOpen())
        {
            file.write(ui->editor->toPlainText().toLatin1());
            file.close();
        }
    }
}

void Writer::undo()
{
    ui->editor->undo();
}

void Writer::redo()
{
    ui->editor->redo();
}

void Writer::textChanged()
{
    ui->savedLabel->setVisible(false);
    timer->stop();
    timer->start();
}

void Writer::deletePressed()
{
    emit requestDelete(note);
}

void Writer::manualSave()
{
    timer->stop();
    save();
}
