#include "texteditor.h"
#include "linenumberarea.h"
#include "qfileinfo.h"
#include <QPainter>
#include <QTextBlock>

TextEditor::TextEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    init();
}

TextEditor::TextEditor(const QString &text, const QString &path, QWidget *parent)  : QPlainTextEdit(parent)
{
    init();
    setPlainText(text);
    this->path = path;
    this->setName(QFileInfo(path).fileName());
    oldContent = text;
    setSaved(true);
}

void TextEditor::init()
{
    lineNumberArea = new LineNumberArea(this);
    connect(this, &TextEditor::blockCountChanged, this, &TextEditor::updateLineNumberWidth);
    connect(this, &TextEditor::updateRequest, this, &TextEditor::updateLineNumber);
    connect(this, &TextEditor::textChanged, this, [this]{setSaved(false);});
    updateLineNumberWidth(0);
    defaultFormat = textCursor().charFormat();
    highlighter = new TextHighlighter(this->document());
}

QString TextEditor::getPath() const
{
    return path;
}

void TextEditor::setPath(const QString &newPath)
{
    path = newPath;
}

int TextEditor::lineNumberWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}
void TextEditor::lineNumberAreaPaint(QPaintEvent* event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor::fromRgb(27, 25, 26));
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor::fromRgb(148, 138, 138));
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void TextEditor::updateLineNumberWidth(int)
{
    setViewportMargins(lineNumberWidth(), 0, 0, 0);
}

void TextEditor::updateLineNumber(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    if (rect.contains(viewport()->rect()))
        updateLineNumberWidth(0);
}

void TextEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberWidth(), cr.height()));
}

QString TextEditor::getName() const
{
    return name;
}

void TextEditor::setName(const QString &newName)
{
    name = newName;
}

void TextEditor::find(const QString &text)
{
    highlighter->setPattern(text);
    highlighter->rehighlight();
}

void TextEditor::replace(const QString &find, const QString &replace)
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(0);
    while(true)
    {
        cursor = document()->find(find, cursor.position());
        if(cursor.isNull())
        {
            break;
        }
        cursor.removeSelectedText();
        cursor.insertText(replace);
        setTextCursor(cursor);
    }
}

void TextEditor::clearSearchFormatting()
{
    highlighter->setPattern("");
    highlighter->rehighlight();
}

bool TextEditor::getSaved() const
{
    return saved;
}

void TextEditor::setSaved(bool newSaved)
{
    saved = newSaved;
}

bool TextEditor::isSaved()
{
    if(!oldContent.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        if(file.isOpen())
        {
            QString content = file.readAll();
            file.close();
            if(oldContent.compare(content))
            {
                setSaved(true);
                return true;
            }
        }
    }
    else if(toPlainText().isEmpty())
        setSaved(true);
    return getSaved();
}
