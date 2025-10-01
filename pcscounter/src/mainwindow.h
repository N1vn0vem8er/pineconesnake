#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <qlabel.h>

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
    struct Counter{
        Counter(){}
        Counter(const QString& name) : name(name){}
        int currentValue {0};
        int step {1};
        QString name;
    };
    Ui::MainWindow *ui;
    int currentCounter {0};
    QList<Counter> counters;
    QStringListModel* model {nullptr};
    QLabel* counterNameLabel {nullptr};

private:
    void updateModel();
    void updateCounter();

private slots:
    void increase();
    void decrease();
    void reset();
    void addCounter();
    void removeCounter();
    void openCounter(const QModelIndex& index);
    void setStep();
    void showAbout();
    void next();
    void previous();
};
#endif // MAINWINDOW_H
