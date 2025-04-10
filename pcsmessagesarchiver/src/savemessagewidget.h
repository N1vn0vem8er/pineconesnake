#ifndef SAVEMESSAGEWIDGET_H
#define SAVEMESSAGEWIDGET_H

#include <QWidget>

namespace Ui {
class SaveMessageWidget;
}

class SaveMessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SaveMessageWidget(QWidget *parent = nullptr);
    ~SaveMessageWidget();

private slots:
    void saveMessage();

private:
    Ui::SaveMessageWidget *ui;
};

#endif // SAVEMESSAGEWIDGET_H
