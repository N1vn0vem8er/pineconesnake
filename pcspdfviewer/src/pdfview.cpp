#include "pdfview.h"

#include <QWheelEvent>

PdfView::PdfView(QWidget *parent) : QPdfView(parent) {}

void PdfView::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier)
    {
        if(event->angleDelta().y() > 0)
        {
            setZoomFactor(zoomFactor() + 0.1);
        }
        else
        {
            if(zoomFactor() - 0.1 > 0)
                setZoomFactor(zoomFactor() - 0.1);
        }
        event->accept();
    }
    else
        QPdfView::wheelEvent(event);
}
