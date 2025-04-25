#include "notecardwidget.h"
#include "inputtitledialog.h"
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
    connect(ui->renameButton, &QPushButton::clicked, this, &NoteCardWidget::editTitle);
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

void NoteCardWidget::editTitle()
{
    InputTitleDialog dialog;
    if(dialog.exec() == QDialog::Accepted)
    {
        note.title = dialog.getTitle();
        ResourcesManager::getInstance()->editNote(note);
        emit requestRefresh();
    }
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
