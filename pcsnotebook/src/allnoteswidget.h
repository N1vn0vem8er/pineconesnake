#ifndef ALLNOTESWIDGET_H
#define ALLNOTESWIDGET_H

#include "structs.h"

#include <QWidget>

namespace Ui {
class AllNotesWidget;
}

class AllNotesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AllNotesWidget(QWidget *parent = nullptr);
    ~AllNotesWidget();

public slots:
    void all();

private:
    Ui::AllNotesWidget *ui;
    QList<Note> notes;
    void refresh();

private slots:
    void search(const QString &text);
    void openNote(const Note& note);

signals:
    void openWriter(const Note& note);
};

#endif // ALLNOTESWIDGET_H
