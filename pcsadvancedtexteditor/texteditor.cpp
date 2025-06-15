#include "texteditor.h"
#include "linenumberarea.h"
#include "qfileinfo.h"
#include "settings.h"
#include <QPainter>
#include <QTextBlock>
#include "qabstractitemview.h"
#include <QKeyEvent>
#include <QScrollBar>
#include <QStringListModel>

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

TextEditor::~TextEditor()
{
    delete model;
}

void TextEditor::init()
{
    lineNumberArea = new LineNumberArea(this);
    connect(this, &TextEditor::blockCountChanged, this, &TextEditor::updateLineNumberWidth);
    connect(this, &TextEditor::updateRequest, this, &TextEditor::updateLineNumber);
    connect(this, &TextEditor::textChanged, this, [this]{setSaved(false);});
    connect(this, &TextEditor::textChanged, this, &TextEditor::checkSpelling);
    updateLineNumberWidth(0);
    defaultFormat = textCursor().charFormat();
    highlighter = new TextHighlighter(this->document());
    spellChecker = std::make_unique<Hunspell>(QString("/usr/share/hunspell/%1.aff").arg(Settings::defaultLanguage).toStdString().c_str(),
                                QString("/usr/share/hunspell/%1.dic").arg(Settings::defaultLanguage).toStdString().c_str());

    completer = std::make_unique<QCompleter>(QStringList(), this);
    completer->setWidget(this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseSensitive);
    completer->setWrapAround(false);
    connect(completer.get(), QOverload<const QString&>::of(&QCompleter::activated), this, &TextEditor::insertCompletion);
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

void TextEditor::checkSpelling()
{
    QTextCursor oldCursor = textCursor();
    QTextCursor cursor(document());

    setExtraSelections({});
    QCoreApplication::processEvents();

    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(QBrush(QColor("#ff6060")));
    highlightFormat.setForeground(QBrush(QColor("#000000")));

    while(!cursor.atEnd())
    {
        QCoreApplication::processEvents();
        cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor, 1);
        QString word = cursor.selectedText();

        while(!word.isEmpty() && !word.at(0).isLetter() && cursor.anchor() < cursor.position()) {
            int cursorPos = cursor.position();
            cursor.setPosition(cursor.anchor() + 1, QTextCursor::MoveAnchor);
            cursor.setPosition(cursorPos, QTextCursor::KeepAnchor);
            word = cursor.selectedText();
        }
        if(!word.isEmpty() && !spellChecker->spell(word.toStdString())) {
            QTextCursor tmpCursor(cursor);
            tmpCursor.setPosition(cursor.anchor());
            setTextCursor(tmpCursor);
            ensureCursorVisible();

            QStringList wordsList;
            for(const auto& i : spellChecker->suggest(word.toStdString()))
            {
                wordsList.append(QString::fromStdString(i));
            }

            delete completer->model();
            model = new QStringListModel(wordsList);
            completer->setModel(model);

            QTextEdit::ExtraSelection es;
            es.cursor = cursor;
            es.format = highlightFormat;

            QList<QTextEdit::ExtraSelection> esList;
            esList << es;
            setExtraSelections(esList);
            QCoreApplication::processEvents();

        }
        cursor.movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor, 1);
    }
    setTextCursor(oldCursor);
}

void TextEditor::insertCompletion(const QString &completion)
{
    QTextCursor tc = textCursor();
    int extra = completion.length() - completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

void TextEditor::keyPressEvent(QKeyEvent *event)
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
