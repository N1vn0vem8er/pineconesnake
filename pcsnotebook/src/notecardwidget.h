#ifndef NOTECARDWIDGET_H
#define NOTECARDWIDGET_H

#include "structs.h"
#include <QWidget>

namespace Ui {
class NoteCardWidget;
}

class NoteCardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NoteCardWidget(const Note& note, QWidget *parent = nullptr);
    ~NoteCardWidget();

private:
    Ui::NoteCardWidget *ui;
    Note note;
};

#endif // NOTECARDWIDGET_H
