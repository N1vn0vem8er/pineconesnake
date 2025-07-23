#ifndef WRITER_H
#define WRITER_H

#include "globals.h"
#include "qcombobox.h"
#include <QWidget>
#include <QCompleter>

namespace Ui {
class Writer;
}

class Writer : public QWidget
{
    Q_OBJECT

public:
    explicit Writer(QWidget *parent = nullptr);
    void clear();
    void setTitle(const QString& title);
    void setFrom(const QString& from);
    void setTo(const QString& to);
    void setContent(const QString& content);
    QString getTitle() const;
    QString getFrom() const;
    QString getTo() const;
    QString getContent() const;
    Globals::Message getMessage();
    ~Writer();

signals:
    void save();

private:
    Ui::Writer *ui;
    QCompleter* completer = nullptr;

private slots:
    void changeFort();
    void saveToFile();
    void undo();
    void redo();
    void copyAll();
};

#endif // WRITER_H
