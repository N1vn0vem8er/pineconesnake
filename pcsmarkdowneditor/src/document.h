#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>

class Document : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER m_text NOTIFY textChanged FINAL)
public:
    explicit Document(QObject *parent = nullptr);
    void setText(const QString& text);

private:
    QString m_text;

signals:
    void textChanged(const QString& text);
};

#endif // DOCUMENT_H
