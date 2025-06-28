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

signals:
    void newFile();
    void openFile();
    void openDir();
};

#endif // STARTWIDGET_H
