#ifndef ARTISTITEMWIDGET_H
#define ARTISTITEMWIDGET_H

#include <QWidget>

class ArtistItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ArtistItemWidget(const QString& title, QWidget *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QString title;

signals:
    void doubleClicked(const QString& title);
};

#endif // ARTISTITEMWIDGET_H
