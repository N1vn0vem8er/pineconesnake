#ifndef PLAYLISTITEMWIDGET_H
#define PLAYLISTITEMWIDGET_H

#include <QWidget>


class PlaylistItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaylistItemWidget(const QString& name, QWidget *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QString name;
private slots:
    void playPressed();
    void deletePlaylist();

signals:
    void doubleClicked(const QString& name);
    void play(const QString& name);
    void del(const QString& name);

};

#endif // PLAYLISTITEMWIDGET_H
