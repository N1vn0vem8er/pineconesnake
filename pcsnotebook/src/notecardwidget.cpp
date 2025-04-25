#include "notecardwidget.h"
#include "resourcesmanager.h"
#include "ui_notecardwidget.h"

#include <QMessageBox>

NoteCardWidget::NoteCardWidget(const Note &note, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NoteCardWidget)
{
    ui->setupUi(this);
    connect(ui->editButton, &QPushButton::clicked, this, &NoteCardWidget::openPressed);
    connect(ui->deleteButton, &QPushButton::clicked, this, &NoteCardWidget::deletePressed);
    this->note = note;
    ui->titleLabel->setText(note.title);
    ui->contentLabel->setText(note.content.left(500));
}

NoteCardWidget::~NoteCardWidget()
{
    delete ui;
}

void NoteCardWidget::openPressed()
{
    emit openNote(note);
}

void NoteCardWidget::deletePressed()
{
    QMessageBox::StandardButton dialog = QMessageBox::question(this, tr("Delete note?"), tr("Do you want to delete %1?").arg(note.title), QMessageBox::Yes | QMessageBox::No);
    if(dialog == QMessageBox::Yes)
    {
        ResourcesManager::getInstance()->deleteNote(note);
    }
    emit requestRefresh();
}
