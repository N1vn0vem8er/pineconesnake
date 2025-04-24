#include "notecardwidget.h"
#include "ui_notecardwidget.h"

NoteCardWidget::NoteCardWidget(const Note &note, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NoteCardWidget)
{
    ui->setupUi(this);
    this->note = note;
    ui->titleLabel->setText(note.title);
    ui->contentLabel->setText(note.content.left(500));
}

NoteCardWidget::~NoteCardWidget()
{
    delete ui;
}
