#include "document.h"

Document::Document(QObject *parent)
    : QObject{parent}
{}

void Document::setText(const QString &text)
{
    if(m_text == text)
        return;
    m_text = text;
    emit textChanged(m_text);
}
