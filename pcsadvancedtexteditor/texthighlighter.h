#ifndef TEXTHIGHLIGHTER_H
#define TEXTHIGHLIGHTER_H

#include "qregularexpression.h"
#include <QSyntaxHighlighter>

class TextHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit TextHighlighter(QTextDocument *parent = nullptr);
    void setPattern(const QString& pattern);

private:
    QRegularExpression pattern;
    QTextCharFormat format;

protected:
    void highlightBlock(const QString &text) override;
};

#endif // TEXTHIGHLIGHTER_H
