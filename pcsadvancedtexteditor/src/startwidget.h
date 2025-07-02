#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include <qstringlistmodel.h>

namespace Ui {
class StartWidget;
}

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartWidget(QWidget *parent = nullptr);
    ~StartWidget();

public slots:
    void refresh();

private:
    Ui::StartWidget *ui;
    QStringListModel* recentFilesModel {nullptr};
    QStringListModel* recentDirsModel {nullptr};
    void loadRecentFiles();
    void loadRecentDirs();
    void clearRecentFiles();
    void clearRecentDirs();

private slots:
    void openFileFromRecentPressed(const QModelIndex &index);
    void openDirFromRecentPressed(const QModelIndex &index);

signals:
    void newFile();
    void openFile();
    void openDir();
    void openFileFromRecent(const QString& path);
    void openDirFromRecent(const QString& dir);
    void recentFilesCleared();
    void recentDirsCleared();
};

#endif // STARTWIDGET_H
