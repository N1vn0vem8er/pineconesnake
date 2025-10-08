#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include "codeeditor.h"
#include <QWidget>

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor* parent = nullptr);
public:
    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    CodeEditor* codeEditor;
};

#endif // LINENUMBERAREA_H
