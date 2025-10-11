#ifndef MARKDOWNSYNTAXHIGHLIGHTER_H
#define MARKDOWNSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <qregularexpression.h>

class MarkdownSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit MarkdownSyntaxHighlighter(QTextDocument* parent = nullptr);

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> highlightingRules;

protected:
    void highlightBlock(const QString &text) override;
};

#endif // MARKDOWNSYNTAXHIGHLIGHTER_H
