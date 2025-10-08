#include "codeeditor.h"
#include "linenumberarea.h"
#include <QPainter>
#include <QTextBlock>
#include <QFuture>
#include <QKeyEvent>
#include <QScrollBar>
#include <QStringListModel>
#include <qabstractitemview.h>
#include <QFile>
#include <QMimeData>
#include <QRegularExpressionMatchIterator>
#include <QFileInfo>

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    init();
}

CodeEditor::CodeEditor(const QString &text, const QString &path, QWidget *parent)  : QPlainTextEdit(parent)
{
    init();
    setPlainText(text);
    this->path = path;
    this->setName(QFileInfo(path).fileName());
}

CodeEditor::~CodeEditor()
{

}

void CodeEditor::init()
{
    setAcceptDrops(true);
    setMouseTracking(true);
    lineNumberArea = new LineNumberArea(this);
    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumber);
    updateLineNumberWidth(0);

    completer = new QCompleter(QStringList(), this);
    completer->setWidget(this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseSensitive);
    completer->setWrapAround(false);
    connect(completer, QOverload<const QModelIndex&>::of(&QCompleter::activated), this, &CodeEditor::insertCompletion);
    completerModel = new QStringListModel(this);
    completer->setModel(completerModel);
}

QString CodeEditor::getPath() const
{
    return path;
}

void CodeEditor::setPath(const QString &newPath)
{
    path = newPath;
}

int CodeEditor::lineNumberWidth()
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
void CodeEditor::lineNumberAreaPaint(QPaintEvent* event)
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

void CodeEditor::updateLineNumberWidth(int)
{
    setViewportMargins(lineNumberWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumber(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    if (rect.contains(viewport()->rect()))
        updateLineNumberWidth(0);
}

bool CodeEditor::getReplaceTabWithSpaces() const
{
    return replaceTabWithSpaces;
}

bool CodeEditor::getAutoClosing() const
{
    return autoClosing;
}

void CodeEditor::insertCompletion(const QModelIndex &index)
{

}

void CodeEditor::mergeSelectedLines()
{
    QTextCursor cursor = textCursor();
    if(cursor.hasSelection())
    {
        cursor.insertText(cursor.selectedText().replace(QChar(8233), ' '));
        setTextCursor(cursor);
    }
}

void CodeEditor::makeSelectedSmall()
{
    QTextCursor cursor = textCursor();
    if(cursor.hasSelection())
    {
        cursor.insertText(cursor.selectedText().toLower());
        setTextCursor(cursor);
    }
}

void CodeEditor::makeSelectedCapital()
{
    QTextCursor cursor = textCursor();
    if(cursor.hasSelection())
    {
        cursor.insertText(cursor.selectedText().toUpper());
        setTextCursor(cursor);
    }
}

void CodeEditor::makeSelectedSentenceCapital()
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

void CodeEditor::makeEverySelectedCapital()
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

void CodeEditor::increaseFontSize()
{
    QFont font = this->font();
    font.setPointSize(font.pointSize() + 1);
    setFont(font);
    emit fontSizeChanged(font.pointSize());
}

void CodeEditor::decreaseFontSize()
{
    QFont font = this->font();
    font.setPointSize(font.pointSize() - 1);
    setFont(font);
    emit fontSizeChanged(font.pointSize());
}

void CodeEditor::setFontSize(int size)
{
    QFont font = this->font();
    font.setPointSize(size);
    setFont(font);
    emit fontSizeChanged(font.pointSize());
}

void CodeEditor::setReplaceTabWithSpacesEnabled(bool val)
{
    replaceTabWithSpaces = val;
}

void CodeEditor::setAutoClosingEnabled(bool val)
{
    autoClosing = val;
}

void CodeEditor::copyAll()
{
    selectAll();
    copy();
}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    if(completer->popup()->isVisible())
    {
        switch (event->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            event->ignore();
            return;
            break;
        default:
            break;
        }
    }
    else if(replaceTabWithSpaces || autoClosing)
    {
        switch(event->key())
        {
        case Qt::Key_BraceLeft:
        {
            if(!autoClosing)
                return;
            QPlainTextEdit::keyPressEvent(event);
            QTextCursor tc = textCursor();
            tc.insertText("}");
            tc.movePosition(QTextCursor::Left);
            setTextCursor(tc);
        }
            return;
        case Qt::Key_BracketLeft:
        {
            if(!autoClosing)
                return;
            QPlainTextEdit::keyPressEvent(event);
            QTextCursor tc = textCursor();
            tc.insertText("]");
            tc.movePosition(QTextCursor::Left);
            setTextCursor(tc);
        }
            return;
        case Qt::Key_Tab:
        {

            if(!replaceTabWithSpaces)
                return;
            QTextCursor tc = textCursor();
            tc.insertText("    ");
            setTextCursor(tc);
        }
            return;
        case Qt::Key_Enter:
        case Qt::Key_Return:
        {
            if(!autoClosing)
                return;
            QPlainTextEdit::keyPressEvent(event);
            QTextCursor tc = textCursor();
            int cursorPosition = tc.position();
            const QString text = toPlainText();
            int depth = 0;
            bool isSingleLineComment = false;
            bool isMultiLineComment = false;
            bool isString = false;
            bool isChar = false;
            for(int i = cursorPosition - 1; i >= 0; i--)
            {
                const QChar currentChar = text.at(i);
                const QChar prevChar = i > 0 ? text.at(i-1) : QChar();

                if(isString)
                {
                    if(currentChar == '"' && prevChar != '\\')
                        isString = false;
                    continue;
                }
                if(isChar)
                {
                    if(currentChar == '\'' && prevChar != '\\')
                        isChar = false;
                    continue;
                }
                if (isMultiLineComment)
                {
                    if (currentChar == '*' && prevChar == '/')
                    {
                        isMultiLineComment = false;
                    }
                    continue;
                }
                if (isSingleLineComment)
                {
                    if (currentChar == '\n')
                    {
                        isSingleLineComment = false;
                    }
                    continue;
                }
                if (currentChar == '/' && prevChar == '/')
                {
                    isSingleLineComment = true;
                    continue;
                }
                else if (currentChar == '*' && prevChar == '/')
                {
                    isMultiLineComment = true;
                    continue;
                }
                else if (currentChar == '"')
                {
                    isString = true;
                    continue;
                }
                else if (currentChar == '\'')
                {
                    isChar = true;
                    continue;
                }
                if (currentChar == '{')
                {
                    depth++;
                }
                else if (currentChar == '}')
                {
                    depth--;
                }
            }
            for(int i=0; i<depth; i++)
                tc.insertText("    ");
            setTextCursor(tc);
        }
            return;
        case Qt::Key_Backspace:
        {
            if(!replaceTabWithSpaces)
                return;
            QTextCursor tc = textCursor();
            tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 4);
            if(tc.selectedText() == "    ")
            {
                tc.removeSelectedText();
                setTextCursor(tc);
            }
            else
            {
                QPlainTextEdit::keyPressEvent(event);
            }
        }
            return;
        }

    }
    const bool isShortcut = (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_E);
    if(!isShortcut)
    {
        QPlainTextEdit::keyPressEvent(event);
    }
    const bool ctrlOrShift = event->modifiers().testFlag(Qt::ControlModifier) || event->modifiers().testFlag(Qt::ShiftModifier);
    if(ctrlOrShift && event->text().isEmpty())
        return;
    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");
    const bool hasModifier = (event->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    QString completionPrefix = tc.selectedText();

    if (!isShortcut && (hasModifier || event->text().isEmpty()|| completionPrefix.length() < 1 || eow.contains(event->text().right(1))))
    {
        completer->popup()->hide();
        return;
    }
    if(completionPrefix != completer->completionPrefix())
    {
        completer->setCompletionPrefix(completionPrefix);
        completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(completer->popup()->sizeHintForColumn(0) + completer->popup()->verticalScrollBar()->sizeHint().width());
    completer->complete(cr);
}

void CodeEditor::dropEvent(QDropEvent *event)
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

void CodeEditor::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void CodeEditor::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void CodeEditor::wheelEvent(QWheelEvent *event)
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

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberWidth(), cr.height()));
}

QString CodeEditor::getName() const
{
    return name;
}

void CodeEditor::setName(const QString &newName)
{
    name = newName;
}

void CodeEditor::highlightTextSequence(const QString& text, bool caseSensitive)
{
    QList<QTextEdit::ExtraSelection> found;
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::darkBlue);
    QRegularExpressionMatchIterator i = QRegularExpression(caseSensitive ? text : text.toUpper()).globalMatch(caseSensitive ? toPlainText() : toPlainText().toUpper());
    while(i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QTextEdit::ExtraSelection es;
        es.format = highlightFormat;
        es.cursor = textCursor();
        es.cursor.setPosition(match.capturedStart());
        es.cursor.setPosition(match.capturedEnd(), QTextCursor::KeepAnchor);
        found.append(es);
    }
    setExtraSelections(found);
}

void CodeEditor::replace(const QString &find, const QString &replace)
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

void CodeEditor::deleteSelected()
{
    textCursor().removeSelectedText();
}

void CodeEditor::deleteAll()
{
    selectAll();
    textCursor().removeSelectedText();
}
