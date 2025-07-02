#include "linenumberarea.h"

LineNumberArea::LineNumberArea(TextEditor *parent) : QWidget(parent), textEditor(parent)
{

}
QSize LineNumberArea::sizeHint() const
{
    return QSize(textEditor->lineNumberWidth(), 0);
}
void LineNumberArea::paintEvent(QPaintEvent *event)
{
    textEditor->lineNumberAreaPaint(event);
}
