#ifndef WRITER_H
#define WRITER_H

#include "structs.h"

#include <QWidget>

namespace Ui {
class Writer;
}

class Writer : public QWidget
{
    Q_OBJECT

public:
    explicit Writer(const Note& note, QWidget *parent = nullptr);
    ~Writer();

public slots:
    void save();

private:
    Ui::Writer *ui;
    Note note;
    QTimer* timer = nullptr;

private slots:
    void textChanged();
    void deletePressed();

signals:
    void requestDelete();

};

#endif // WRITER_H
