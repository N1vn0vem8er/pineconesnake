#include "spellcheckerworker.h"

#include <QThread>
#include <qregularexpression.h>

SpellCheckerWorker::SpellCheckerWorker(QTextDocument* document, std::shared_ptr<Hunspell> hunspell, QObject *parent)
    : QObject{parent}
{
    cursor = QTextCursor(document);
    text = document->toPlainText();
    spellChecker = hunspell;
}

void SpellCheckerWorker::spellCheck()
{
    QTextCharFormat highlightFormat;
    highlightFormat.setUnderlineColor(QColor::fromRgb(255, 0, 0));
    highlightFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    QList<QTextEdit::ExtraSelection> esList;
    QRegularExpressionMatchIterator i = QRegularExpression(R"([\p{L}\p{N}\p{M}_]+)").globalMatch(text);
    QSet<QString> knownCorrectWords;
    QSet<QString> knownIncorrectWords;

    while(i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString word = match.captured(0);
        if (knownCorrectWords.contains(word))
        {
            continue;
        }

        bool isMisspelled = false;
        if (knownIncorrectWords.contains(word))
        {
            isMisspelled = true;
        }
        else
        {
            if(!spellChecker->spell(word.toStdString()))
            {
                knownIncorrectWords.insert(word);
                isMisspelled = true;
            }
            else
            {
                knownCorrectWords.insert(word);
            }
        }

        if(isMisspelled)
        {
            QTextEdit::ExtraSelection es;
            es.format = highlightFormat;
            es.cursor = cursor;
            es.cursor.setPosition(match.capturedStart());
            es.cursor.setPosition(match.capturedEnd(), QTextCursor::KeepAnchor);

            esList << es;
        }
    }

    emit resultReady(esList);
    emit finished();
}
