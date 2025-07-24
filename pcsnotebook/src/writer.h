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
    void appendText(const QString& text);

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
    void increaseFontSize();
    void decreaseFontSize();
    void setFontSize(int size);
    void mergeSelectedLines();
    void makeSelectedSmall();
    void makeSelectedCapital();
    void makeSelectedSentenceCapital();
    void makeEverySelectedCapital();
    void setLineWrap(bool val);

private:
    Ui::Writer *ui;
    Note note;
    QTimer* timer = nullptr;

private slots:
    void textChanged();
    void deletePressed();

signals:
    void requestDelete(const Note& note);
    void changed();

};

#endif // WRITER_H
