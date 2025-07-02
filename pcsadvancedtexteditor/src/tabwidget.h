#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QWidget>

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // TABWIDGET_H
