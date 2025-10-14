#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "codeeditor.h"
#include "document.h"
#include "previewpage.h"
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

private:
    Ui::MainWindow *ui;
    Document document;
    PreviewPage* page {nullptr};

private:
    CodeEditor* createEditor(const QString& path, const QString& content);

private slots:
    void newFile();
    void openFile();
    void open(const QString& path);
    void openDir();
    void save();
    void saveAs();
    void closeTab(int index);
    void tabChanged();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void deleteText();
    void deleteAllText();
    void selectAll();
    void showAbout();
    void increaseFontSize();
    void decreaseFontSize();
    void onSetFont();
    void fontSizeChanged(int size);
    void openFileInPreview(const QString& path);
    void openInEditor(const QString& text, const QString& title, bool readOnly = true, bool spellChecking = false, bool disableSaveWarning = true);
};
#endif // MAINWINDOW_H
