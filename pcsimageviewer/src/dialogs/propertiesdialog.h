#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>

namespace Ui {
class PropertiesDialog;
}

class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropertiesDialog(QString filePath, QWidget *parent = nullptr);
    ~PropertiesDialog();

private:
    Ui::PropertiesDialog *ui;
};

#endif // PROPERTIESDIALOG_H
