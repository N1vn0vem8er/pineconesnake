#ifndef PLAYLISTNAMEDIALOG_H
#define PLAYLISTNAMEDIALOG_H

#include <QDialog>

namespace Ui {
class PlaylistNameDialog;
}

class PlaylistNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistNameDialog(QWidget *parent = nullptr);
    ~PlaylistNameDialog();
    QString getName() const;

private:
    Ui::PlaylistNameDialog *ui;

signals:
    void playlistName(const QString& name);
};

#endif // PLAYLISTNAMEDIALOG_H
