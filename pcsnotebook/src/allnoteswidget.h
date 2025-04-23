#ifndef ALLNOTESWIDGET_H
#define ALLNOTESWIDGET_H

#include <QWidget>

namespace Ui {
class AllNotesWidget;
}

class AllNotesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AllNotesWidget(QWidget *parent = nullptr);
    ~AllNotesWidget();

private:
    Ui::AllNotesWidget *ui;
};

#endif // ALLNOTESWIDGET_H
