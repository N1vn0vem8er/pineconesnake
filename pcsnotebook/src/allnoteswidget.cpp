#include "allnoteswidget.h"
#include "notecardwidget.h"
#include "resourcesmanager.h"
#include "ui_allnoteswidget.h"

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
    if(layout != nullptr)
    {
        QLayoutItem* layoutItem;
        while((layoutItem = layout->takeAt(0)) != nullptr)
        {
            delete layoutItem->widget();
            delete layoutItem;
        }
        delete layout;
        layout = nullptr;
    }
    layout = new QVBoxLayout(ui->contents);
    for(const auto& i : notes)
    {
        NoteCardWidget* widget = new NoteCardWidget(i, this);
        connect(widget, &NoteCardWidget::openNote, this, &AllNotesWidget::openNote);
        connect(widget, &NoteCardWidget::requestRefresh, this, &AllNotesWidget::all);
        layout->addWidget(widget);
    }
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding));
    ui->contents->setLayout(layout);
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
