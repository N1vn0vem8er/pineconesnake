#ifndef PDFVIEW_H
#define PDFVIEW_H

#include <QPdfView>

class PdfView : public QPdfView
{
    Q_OBJECT
public:
    explicit PdfView(QWidget* parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // PDFVIEW_H
