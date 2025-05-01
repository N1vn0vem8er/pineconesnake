#ifndef PLAYINGWIDGET_H
#define PLAYINGWIDGET_H

#include <QWidget>

namespace Ui {
class PlayingWidget;
}

class PlayingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayingWidget(QWidget *parent = nullptr);
    ~PlayingWidget();

private:
    Ui::PlayingWidget *ui;
};

#endif // PLAYINGWIDGET_H
