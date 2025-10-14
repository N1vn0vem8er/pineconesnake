#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "markdownsyntaxhighlighter.h"
#include <QCompleter>
#include <QPlainTextEdit>
#include <QStringListModel>
#include <qfuturewatcher.h>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = nullptr);
    CodeEditor(const QString& text, const QString& path, QWidget* parent = nullptr);
    ~CodeEditor();
    QString getPath() const;
    void setPath(const QString &newPath);
    int lineNumberWidth();
    void lineNumberAreaPaint(QPaintEvent* event);
    QString getName() const;
    void setName(const QString &newName);
    void replace(const QString& find, const QString& replace);
    void deleteSelected();
    void deleteAll();
    void mergeSelectedLines();
    void makeSelectedSmall();
    void makeSelectedCapital();
    void makeSelectedSentenceCapital();
    void makeEverySelectedCapital();
    void increaseFontSize();
    void decreaseFontSize();
    void setFontSize(int size);
    void setReplaceTabWithSpacesEnabled(bool val);
    void setAutoClosingEnabled(bool val);
    bool getAutoClosing() const;
    bool getReplaceTabWithSpaces() const;
    void setSaved(bool val);
    bool isSaved();
    bool getSaveWarningEnabled() const;
    void setSaveWarningEnabled(bool newSaveWarningEnabled);

public slots:
    void copyAll();
    void highlightTextSequence(const QString &text, bool caseSensitive);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void dropEvent(QDropEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QString orginalText;
    QString path;
    QWidget *lineNumberArea;
    QString name;
    void init();
    void updateLineNumberWidth(int count);
    void updateLineNumber(const QRect &rect, int dy);
    QTextCharFormat defaultFormat;
    QCompleter* completer {nullptr};
    QStringListModel* completerModel {nullptr};
    bool autoClosing {true};
    bool replaceTabWithSpaces {true};
    bool saved {true};
    bool gettingWords {false};
    QFutureWatcher<QStringList> wordsWatcher;
    MarkdownSyntaxHighlighter* syntaxHighlighter {nullptr};
    bool saveWarningEnabled {true};

private slots:
    void insertCompletion(const QString &completion);
    void startGettingWords();

signals:
    void fontSizeChanged(int size);
};

#endif // CODEEDITOR_H
