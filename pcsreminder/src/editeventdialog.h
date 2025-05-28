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
};

#endif // EDITEVENTDIALOG_H
