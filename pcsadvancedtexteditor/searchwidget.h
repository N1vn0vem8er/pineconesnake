#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>

namespace Ui {
class SearchWidget;
}

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWidget(QWidget *parent = nullptr);
    ~SearchWidget();

private:
    Ui::SearchWidget *ui;

private slots:
    void find();
    void replace();
    void exchange();

signals:
    void find(const QString& text);
    void replace(const QString& find, const QString& replace);
};

#endif // SEARCHWIDGET_H
