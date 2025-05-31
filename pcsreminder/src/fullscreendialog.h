#ifndef FULLSCREENDIALOG_H
#define FULLSCREENDIALOG_H

#include <QDialog>

namespace Ui {
class FullScreenDialog;
}

class FullScreenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FullScreenDialog(QWidget *parent = nullptr);
    ~FullScreenDialog();

private:
    Ui::FullScreenDialog *ui;
};

#endif // FULLSCREENDIALOG_H
