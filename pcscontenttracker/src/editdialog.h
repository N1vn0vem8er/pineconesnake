#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include "structs.h"
#include <QDialog>

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(Item item, QWidget *parent = nullptr);
    ~EditDialog();
    Item getIten() const;

private:
    Ui::EditDialog *ui;
    Item item;
};

#endif // EDITDIALOG_H
