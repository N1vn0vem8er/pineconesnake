#ifndef SPELLCHECKERWORKER_H
#define SPELLCHECKERWORKER_H

#include <QObject>
#include <hunspell/hunspell.hxx>
#include <mutex>
#include <qtextdocument.h>
#include <qtextedit.h>

class SpellCheckerWorker : public QObject
{
    Q_OBJECT
public:
    explicit SpellCheckerWorker(const QString& text, std::shared_ptr<Hunspell> hunspell, std::mutex& mutex, QObject *parent = nullptr);
    ~SpellCheckerWorker();

public slots:
    void spellCheck();

private:
    std::shared_ptr<Hunspell> spellChecker = nullptr;
    QString text;
    std::mutex& mutex;

signals:
    void resultReady(const QList<QPair<int,int>>& list);
    void finished();
};

#endif // SPELLCHECKERWORKER_H
