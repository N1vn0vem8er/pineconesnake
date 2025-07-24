#include "allnoteswidget.h"
#include "notecardwidget.h"
#include "resourcesmanager.h"
#include "ui_allnoteswidget.h"
#include <QScrollBar>

AllNotesWidget::AllNotesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AllNotesWidget)
{
    ui->setupUi(this);
    connect(ui->searchLine, &QLineEdit::textChanged, this, &AllNotesWidget::search);
    all();
}

AllNotesWidget::~AllNotesWidget()
{
    delete ui;
}

void AllNotesWidget::refresh()
{
    int scrollPosition = ui->listWidget->verticalScrollBar()->value();
    ui->listWidget->clear();
    for(const auto& i : std::as_const(notes))
    {
        NoteCardWidget* widget = new NoteCardWidget(i, this);
        connect(widget, &NoteCardWidget::openNote, this, &AllNotesWidget::openNote);
        connect(widget, &NoteCardWidget::requestRefresh, this, &AllNotesWidget::all);
        QListWidgetItem* listItem = new QListWidgetItem(ui->listWidget);
        listItem->setSizeHint(widget->sizeHint());
        ui->listWidget->addItem(listItem);
        ui->listWidget->setItemWidget(listItem, widget);
    }
    ui->listWidget->verticalScrollBar()->setValue(scrollPosition);
}

void AllNotesWidget::search(const QString& text)
{
    notes = ResourcesManager::getInstance()->getNotesForTitle(text);
    refresh();
}

void AllNotesWidget::all()
{
    notes = ResourcesManager::getInstance()->getAllNotes();
    refresh();
}

void AllNotesWidget::openNote(const Note &note)
{
    emit openWriter(note);
}
