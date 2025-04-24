#ifndef INPUTTITLEDIALOG_H
#define INPUTTITLEDIALOG_H

#include <QDialog>

namespace Ui {
class InputTitleDialog;
}

class InputTitleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputTitleDialog(QWidget *parent = nullptr);
    ~InputTitleDialog();
    QString getTitle() const;

private:
    Ui::InputTitleDialog *ui;
};

#endif // INPUTTITLEDIALOG_H
