#include "linenumberarea.h"

LineNumberArea::LineNumberArea(CodeEditor *parent) : QWidget(parent), codeEditor(parent)
{

}
QSize LineNumberArea::sizeHint() const
{
    return QSize(codeEditor->lineNumberWidth(), 0);
}
void LineNumberArea::paintEvent(QPaintEvent *event)
{
    codeEditor->lineNumberAreaPaint(event);
}
