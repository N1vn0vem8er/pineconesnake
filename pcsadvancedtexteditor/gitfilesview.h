#ifndef GITFILESVIEW_H
#define GITFILESVIEW_H

#include <QTableView>

class GitFilesView : public QTableView
{
    Q_OBJECT
public:
    explicit GitFilesView(QWidget *parent = nullptr);
    void setContextMenu(QMenu* contextMenu);

private:
    QMenu* contextMenu {nullptr};

private slots:
    void openContextMenu(const QPoint &point);

};

#endif // GITFILESVIEW_H
