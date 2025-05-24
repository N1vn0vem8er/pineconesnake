#ifndef REPEATEDEVENTWIDGET_H
#define REPEATEDEVENTWIDGET_H

#include <QWidget>

namespace Ui {
class RepeatedEventWidget;
}

class RepeatedEventWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RepeatedEventWidget(QWidget *parent = nullptr);
    ~RepeatedEventWidget();

private:
    Ui::RepeatedEventWidget *ui;
};

#endif // REPEATEDEVENTWIDGET_H
