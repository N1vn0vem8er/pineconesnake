#include "markdownsyntaxhighlighter.h"

MarkdownSyntaxHighlighter::MarkdownSyntaxHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    QTextCharFormat format;
    format.setForeground(QColor::fromRgb(200, 0, 77));
    format.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral(R"(^#{1,6}\s.*$)"));
    rule.format = format;
    highlightingRules.append(rule);
    format.setFontWeight(QFont::Normal);
    format.setForeground(QColor::fromRgb(0, 80, 150));
    rule.pattern = QRegularExpression(QStringLiteral(R"((?:\*\*\*|___)(.*?)(?:\*\*\*|___))"));
    rule.format = format;
    highlightingRules.append(rule);
    format.setForeground(QColor::fromRgb(0, 80, 150));
    rule.pattern = QRegularExpression(QStringLiteral(R"((?:\*\*|__)(.*?)(?:\*\*|__))"));
    rule.format = format;
    highlightingRules.append(rule);
    format.setForeground(QColor::fromRgb(0, 80, 150));
    rule.pattern = QRegularExpression(QStringLiteral(R"(\b(?:\*|_)(?!\s)(.*?)(?<!\s)(?:\*|_)\b)"));
    rule.format = format;
    highlightingRules.append(rule);
    format.setForeground(Qt::darkGray);
    rule.pattern = QRegularExpression(QStringLiteral(R"(~~(.*?)~~)"));
    rule.format = format;
    highlightingRules.append(rule);
    format.setForeground(QColor::fromRgb(200, 50, 50));
    rule.pattern = QRegularExpression(QStringLiteral(R"(`(.*?)`)"));
    rule.format = format;
    highlightingRules.append(rule);
    format.setForeground(QColor::fromRgb(0, 128, 255));
    rule.pattern = QRegularExpression(QStringLiteral(R"(!?\[([^\]]*)\]\(([^)]+)\))"));
    rule.format = format;
    highlightingRules.append(rule);
    format.setForeground(QColor::fromRgb(150, 80, 0));
    rule.pattern = QRegularExpression(QStringLiteral(R"(^\s*([\*\-\+]|\d+\.)\s.*)"));
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral(R"(^>\s.*)"));
    highlightingRules.append(rule);

}

void MarkdownSyntaxHighlighter::highlightBlock(const QString &text)
{
    for(const HighlightingRule& rule : std::as_const(highlightingRules))
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while(matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
