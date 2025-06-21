#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>

namespace Ui {
class StartWidget;
}

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartWidget(QWidget *parent = nullptr);
    ~StartWidget();

private:
    Ui::StartWidget *ui;

signals:
    void newFile();
    void openFile();
    void openDir();
};

#endif // STARTWIDGET_H
