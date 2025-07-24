#include "texteditor.h"
#include "linenumberarea.h"
#include "qfileinfo.h"
#include <QMimeData>
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

void TextEditor::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    if(mimeData->hasUrls())
    {
        QFile file(mimeData->urls().at(0).path());
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            appendPlainText(file.readAll());
            file.close();
        }
    }
    else if(mimeData->hasText())
    {
        appendPlainText(mimeData->text());
    }
}

void TextEditor::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void TextEditor::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void TextEditor::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier)
    {
        if(event->angleDelta().y() > 0)
        {
            increaseFontSize();
        }
        else
        {
            decreaseFontSize();
        }
        event->accept();
    }
    else
        QPlainTextEdit::wheelEvent(event);
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
    clearSearchFormatting();
    QTextCursor cursor = textCursor();
    cursor.setPosition(0);
    QTextCharFormat format = QTextCharFormat();
    format.setBackground(Qt::yellow);
    while(true)
    {
        cursor = document()->find(text, cursor.position());
        if(cursor.isNull())
        {
            break;
        }
        cursor.mergeCharFormat(format);
        setTextCursor(cursor);
    }
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
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::Document);
    cursor.setCharFormat(defaultFormat);
    cursor.clearSelection();
    setTextCursor(cursor);
}

void TextEditor::increaseFontSize()
{
    QFont font = this->font();
    font.setPointSize(font.pointSize() + 1);
    setFont(font);
    emit fontSizeChanged(font.pointSize());
}

void TextEditor::decreaseFontSize()
{
    QFont font = this->font();
    font.setPointSize(font.pointSize() - 1);
    setFont(font);
    emit fontSizeChanged(font.pointSize());
}

void TextEditor::setFontSize(int size)
{
    QFont font = this->font();
    font.setPointSize(size);
    setFont(font);
    emit fontSizeChanged(font.pointSize());
}

void TextEditor::mergeSelectedLines()
{
    QTextCursor cursor = textCursor();
    if(cursor.hasSelection())
    {
        cursor.insertText(cursor.selectedText().replace(QChar(8233), ' '));
        setTextCursor(cursor);
    }
}

void TextEditor::makeSelectedSmall()
{
    QTextCursor cursor = textCursor();
    if(cursor.hasSelection())
    {
        cursor.insertText(cursor.selectedText().toLower());
        setTextCursor(cursor);
    }
}

void TextEditor::makeSelectedCapital()
{
    QTextCursor cursor = textCursor();
    if(cursor.hasSelection())
    {
        cursor.insertText(cursor.selectedText().toUpper());
        setTextCursor(cursor);
    }
}

void TextEditor::makeSelectedSentenceCapital()
{
    QTextCursor cursor = textCursor();
    if(cursor.hasSelection())
    {
        cursor.beginEditBlock();
        QString selectedText = cursor.selectedText();
        if(selectedText.isEmpty())
        {
            cursor.endEditBlock();
            return;
        }
        bool capitalNext = true;
        for(auto& character : selectedText)
        {
            if(capitalNext && character.isLetter())
            {
                character = character.toUpper();
                capitalNext = false;
            }
            else if(character == '.' || character == '?' || character == '!')
                capitalNext = true;
            else if(!character.isSpace())
                capitalNext = false;
        }
        cursor.insertText(selectedText);
        cursor.endEditBlock();
        setTextCursor(cursor);
    }
}

void TextEditor::makeEverySelectedCapital()
{
    QTextCursor cursor = textCursor();
    if(cursor.hasSelection())
    {
        cursor.beginEditBlock();
        QString selectedText = cursor.selectedText();
        if(selectedText.isEmpty())
        {
            cursor.endEditBlock();
            return;
        }
        bool capitalNext = true;
        for(auto& character : selectedText)
        {
            if(capitalNext && character.isLetter())
            {
                character = character.toUpper();
                capitalNext = false;
            }
            else if(character.isSpace() )
                capitalNext = true;
        }
        cursor.insertText(selectedText);
        cursor.endEditBlock();
        setTextCursor(cursor);
    }
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
