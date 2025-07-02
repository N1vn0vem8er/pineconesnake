#ifndef SAVEWARNINGDIALOG_H
#define SAVEWARNINGDIALOG_H

#include <QDialog>

namespace Ui {
class SaveWarningDialog;
}

class SaveWarningDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveWarningDialog(QString name, QWidget *parent = nullptr);
    ~SaveWarningDialog();

private:
    Ui::SaveWarningDialog *ui;
};

#endif // SAVEWARNINGDIALOG_H
