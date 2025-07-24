#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
#include <QPlainTextEdit>

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    TextEditor(QWidget *parent = nullptr);
    TextEditor(const QString& text, const QString& path, QWidget* parent = nullptr);
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
    void increaseFontSize();
    void decreaseFontSize();
    void setFontSize(int size);
    void mergeSelectedLines();
    void makeSelectedSmall();
    void makeSelectedCapital();
    void makeSelectedSentenceCapital();
    void makeEverySelectedCapital();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void dropEvent(QDropEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;

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

signals:
    void fontSizeChanged(int size);
};

#endif // TEXTEDITOR_H
