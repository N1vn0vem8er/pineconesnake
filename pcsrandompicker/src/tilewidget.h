#ifndef TILEWIDGET_H
#define TILEWIDGET_H

#include <QWidget>

namespace Ui {
class TileWidget;
}

class TileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TileWidget(QWidget *parent = nullptr);
    ~TileWidget();
    void setText(const QString& text);

private:
    Ui::TileWidget *ui;

private slots:
    void edit();

signals:
    void edited(const QString& oldText, const QString& newText);
};

#endif // TILEWIDGET_H
