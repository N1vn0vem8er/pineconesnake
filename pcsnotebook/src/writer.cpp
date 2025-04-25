#include "writer.h"
#include "qdatetime.h"
#include "ui_writer.h"
#include "resourcesmanager.h"

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
}

void Writer::textChanged()
{
    timer->stop();
    timer->start();
}

void Writer::deletePressed()
{
    emit requestDelete();
}
