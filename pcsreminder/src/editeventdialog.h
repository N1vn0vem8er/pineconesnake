#ifndef EDITEVENTDIALOG_H
#define EDITEVENTDIALOG_H

#include <QDialog>

namespace Ui {
class EditEventDialog;
}

class EditEventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditEventDialog(QWidget *parent = nullptr);
    ~EditEventDialog();

private:
    Ui::EditEventDialog *ui;

private slots:
    void submit();
};

#endif // EDITEVENTDIALOG_H
