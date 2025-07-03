#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QGraphicsView>

class ImageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageView(QGraphicsView *parent = nullptr);
    ~ImageView();
    QImage getImage() const;
    void setImage(const QImage &newImage, const QString& path);
    void rotateRight();
    void setScale(double scale);
    double getScale() const;
    void rotateLeft();
    QString getPath() const;

private:
    QImage image;
    double rotation = 0;
    double imageScale = 1;
    QGraphicsScene* scene = nullptr;
    QGraphicsItemGroup* group = nullptr;
    QGraphicsPixmapItem* item = nullptr;
    QString path;

signals:
    void scaleChanged(double factor);

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // IMAGEVIEW_H
