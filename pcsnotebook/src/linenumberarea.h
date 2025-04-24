#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include "texteditor.h"
#include <QWidget>

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(TextEditor* parent = nullptr);
public:
    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    TextEditor* textEditor;
};

#endif // LINENUMBERAREA_H
