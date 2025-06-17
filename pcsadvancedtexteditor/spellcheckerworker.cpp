#include "spellcheckerworker.h"

#include <QThread>
#include <qregularexpression.h>

SpellCheckerWorker::SpellCheckerWorker(const QString& text, std::shared_ptr<Hunspell> hunspell, QObject *parent)
    : QObject{parent}
{
    this->text = text;
    spellChecker = hunspell;
}

SpellCheckerWorker::~SpellCheckerWorker()
{

}

void SpellCheckerWorker::spellCheck()
{
    QList<QPair<int,int>> ret;
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
            ret << QPair(match.capturedStart(), match.capturedEnd());
        }
    }

    emit resultReady(ret);
    emit finished();
}
