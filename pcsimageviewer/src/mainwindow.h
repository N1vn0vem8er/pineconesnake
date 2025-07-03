#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qfileinfo.h"
#include "qlabel.h"
#include <QMainWindow>

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

public slots:
    void changeScaleSlider(double factor);

private:
    Ui::MainWindow *ui;
    QString openedImage = "";
    QLabel* imagePathLabel;
    int sliderPosition = 0;

    void openImage();
    void openImageInCurrentTab(const QString &path);
    void loadRecentFiles();
    void displayAboutApplication();
    void rotateImageRight();
    void rotateImageLeft();
    void extracted();
    void displayFileProperties();
    void zoom();
    void saveToRecent();
    void loadRecent();
    void open(const QString& path);
    void openImegeLeft();
    void openImageRight();
    QFileInfoList getImagesInDirectory(const QString &path);
    void clearRecent();
    void setVisibility(bool val);
    QString getRecentPath() const;

private slots:
    void closeTab(int index);
    void tabChanged(int index);

};
#endif // MAINWINDOW_H
