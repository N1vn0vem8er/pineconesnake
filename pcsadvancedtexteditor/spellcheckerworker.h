#ifndef SPELLCHECKERWORKER_H
#define SPELLCHECKERWORKER_H

#include <QObject>
#include <hunspell/hunspell.hxx>
#include <qtextdocument.h>
#include <qtextedit.h>

class SpellCheckerWorker : public QObject
{
    Q_OBJECT
public:
    explicit SpellCheckerWorker(QTextDocument *document, std::shared_ptr<Hunspell> hunspell, QObject *parent = nullptr);

public slots:
    void spellCheck();

private:
    QTextCursor cursor;
    std::shared_ptr<Hunspell> spellChecker = nullptr;
    QString text;

signals:
    void resultReady(const QList<QTextEdit::ExtraSelection>& list);
    void finished();
};

#endif // SPELLCHECKERWORKER_H
