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
    QString getText() const;

private:
    Ui::TileWidget *ui;

private slots:
    void edit();
    void remove();

signals:
    void edited(const QString& oldText, const QString& newText);
    void removed(const QString& text);
};

#endif // TILEWIDGET_H
