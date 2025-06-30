#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

protected:
    void dropEvent(QDropEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;

private:
    Ui::MainWindow *ui;
    QLabel* pathLabel {nullptr};
    QLabel* languageLabel {nullptr};
    QThread* mimeFinderThread {nullptr};
    bool hasGitRepository {false};
    void openDir(const QString& path);
    void addTab(QWidget* widget, const QString& title);
    void saveFile(const QString &path, const QString &text);
    void openSaveWarningDialog(const QString path);
    void openFile(const QString &path);
    void loadHunspell();
    void startGettingMimeData();
    void saveFileToRecent(const QString& path);
    void saveDirToRecent(const QString& path);

private slots:
    void newFilePressed();
    void openFilePressed();
    void openDirPressed();
    void saveAs();
    void save();
    void open();
    void openWithText(const QString& text, const QString &title, bool readOnly, bool spellChecking);
    void closeTab(const int index);
    void tabChanged(const int index);
    void copy();
    void selectAll();
    void cut();
    void paste();
    void undo();
    void redo();
    void showSearch();
    void find(const QString& text);
    void replace(const QString& find, const QString& replace);
    void deleteSelected();
    void deleteAll();
    void spellCheckSwitch(bool val);
    void changeLanguageForEditor(const QString& language);
    void showStart();
    void closeAllButThis();
    void reloadCurrent();
    void reloadAll();
    void openPrint();
    void overwriteModeChanged(bool val);
    void readOnlyChanget(bool val);
    void mergeLines();
    void makeSmallLetters();
    void makeCapitalLetters();
    void makeCapitalSentences();
    void makeCapitalEveryWord();

signals:
    void startMimeSearch();
    void refreshStartWidgets();
};
#endif // MAINWINDOW_H
