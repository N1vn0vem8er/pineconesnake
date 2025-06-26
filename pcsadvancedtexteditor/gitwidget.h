#ifndef GITWIDGET_H
#define GITWIDGET_H

#include <QWidget>

namespace Ui {
class GitWidget;
}

class GitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GitWidget(QWidget *parent = nullptr);
    ~GitWidget();

private:
    Ui::GitWidget *ui;
    void setVisibility(bool val);
};

#endif // GITWIDGET_H
