#ifndef ADDNEWDIALOG_H
#define ADDNEWDIALOG_H

#include "structs.h"
#include <QDialog>

namespace Ui {
class AddNewDialog;
}

class AddNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewDialog(QWidget *parent = nullptr);
    ~AddNewDialog();
    Item getItem() const;

private:
    Ui::AddNewDialog *ui;
private slots:
    void openSelectImage();
};

#endif // ADDNEWDIALOG_H
