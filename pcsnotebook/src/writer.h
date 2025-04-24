#ifndef WRITER_H
#define WRITER_H

#include <QWidget>

namespace Ui {
class Writer;
}

class Writer : public QWidget
{
    Q_OBJECT

public:
    explicit Writer(QWidget *parent = nullptr);
    ~Writer();

private:
    Ui::Writer *ui;
};

#endif // WRITER_H
