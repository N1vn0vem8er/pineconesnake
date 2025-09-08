#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qboxlayout.h>
#include <qpdfview.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void closeTab(int index);
    void tabChanged();
    void open();
    void bookmarkSelected(const QModelIndex& index);
    void zoomIn();
    void zoomOut();
    void zoomChanged(qreal zoom);
};
#endif // MAINWINDOW_H
