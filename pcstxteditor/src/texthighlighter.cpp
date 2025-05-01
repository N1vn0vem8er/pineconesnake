#include "texthighlighter.h"

#include <QRegularExpressionMatchIterator>

TextHighlighter::TextHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter{parent}
{
    format.setForeground(Qt::yellow);
}

void TextHighlighter::setPattern(const QString &pattern)
{
    this->pattern = QRegularExpression(pattern);
}

void TextHighlighter::highlightBlock(const QString &text)
{
    QRegularExpressionMatchIterator matchIterator = pattern.globalMatch(text);
    while(matchIterator.hasNext())
    {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), format);
    }
}
