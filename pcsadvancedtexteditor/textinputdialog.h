#ifndef TEXTINPUTDIALOG_H
#define TEXTINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class TextInputDialog;
}

class TextInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextInputDialog(QWidget *parent = nullptr);
    ~TextInputDialog();
    QString getText() const;

private:
    Ui::TextInputDialog *ui;
};

#endif // TEXTINPUTDIALOG_H
