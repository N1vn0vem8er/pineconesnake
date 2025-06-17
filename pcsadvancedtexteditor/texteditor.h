#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
#include "texthighlighter.h"
#include <QPlainTextEdit>
#include <hunspell/hunspell.hxx>
#include <QCompleter>
#include <qstringlistmodel.h>

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    TextEditor(QWidget *parent = nullptr);
    TextEditor(const QString& text, const QString& path, QWidget* parent = nullptr);
    ~TextEditor();
    QString getPath() const;
    void setPath(const QString &newPath);
    int lineNumberWidth();
    void lineNumberAreaPaint(QPaintEvent* event);
    bool getSaved() const;
    void setSaved(bool newSaved);
    bool isSaved();
    QString getName() const;
    void setName(const QString &newName);
    void find(const QString& text);
    void replace(const QString& find, const QString& replace);
    void clearSearchFormatting();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QString path;
    QWidget *lineNumberArea;
    bool saved = true;
    QString name;
    QString oldContent = "";
    void init();
    void updateLineNumberWidth(int count);
    void updateLineNumber(const QRect &rect, int dy);
    QTextCharFormat defaultFormat;
    TextHighlighter* highlighter = nullptr;
    std::shared_ptr<Hunspell> spellChecker;
    std::unique_ptr<QCompleter> completer;
    QStringListModel* model = nullptr;
    QThread* spellcheckThread = nullptr;
    QTimer* timer = nullptr;

private slots:
    void checkSpelling();
    void insertCompletion(const QString &completion);
    void startSpellChecking();
    void spellCheckResoultsReady(const QList<QPair<int,int>>& list);
signals:
    void startSpellcheck();

};

#endif // TEXTEDITOR_H
