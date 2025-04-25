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

public slots:
    void deletePressed();

private:
    Ui::NoteCardWidget *ui;
    Note note;

private slots:
    void openPressed();

signals:
    void openNote(const Note& note);
    void requestRefresh();
};

#endif // NOTECARDWIDGET_H
