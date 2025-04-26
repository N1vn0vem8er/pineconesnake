#ifndef WRITER_H
#define WRITER_H

#include "structs.h"

#include <QWidget>

namespace Ui {
class Writer;
}

class Writer : public QWidget
{
    Q_OBJECT

public:
    explicit Writer(const Note& note, QWidget *parent = nullptr);
    ~Writer();

public slots:
    void save();
    void copy();
    void cut();
    void paste();
    void selectAll();
    void saveAs();
    void undo();
    void redo();
    void manualSave();

private:
    Ui::Writer *ui;
    Note note;
    QTimer* timer = nullptr;

private slots:
    void textChanged();
    void deletePressed();

signals:
    void requestDelete(const Note& note);

};

#endif // WRITER_H
