#include "imageview.h"
#include "qapplication.h"
#include <QPainter>
#include <QGraphicsItemGroup>
#include <QWheelEvent>

ImageView::ImageView(QGraphicsView *parent)
    : QGraphicsView{parent}{
    setMouseTracking(true);
}

ImageView::~ImageView()
{
    delete item;
    delete group;
    delete scene;
}

QImage ImageView::getImage() const{
    return image;
}

void ImageView::setImage(const QImage &newImage){
    image = newImage;
    if(item != nullptr) delete item;
    if(group != nullptr) delete group;
    if(scene != nullptr) delete scene;
    scene = new QGraphicsScene(this);
    group = new QGraphicsItemGroup();
    item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    group->addToGroup(item);
    scene->addItem(group);
    setScene(scene);
    ensureVisible(scene->sceneRect());
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    repaint();
}

void ImageView::rotateRight()
{
    if(rotation >= 270)
    {
        rotation = 0;
        this->rotate(-270);
    }
    else
    {
        rotation+= 90;
        this->rotate(90);
    }
}

void ImageView::setScale(double scale)
{
    imageScale = scale;
    this->scale(imageScale, imageScale);
}

double ImageView::getScale() const
{
    return imageScale;
}

void ImageView::rotateLeft()
{
    if(rotation >= 270)
    {
        rotation = 0;
        this->rotate(270);
    }
    else
    {
        rotation-= 90;
        this->rotate(-90);
    }
}

void ImageView::paintEvent(QPaintEvent *event){
    QGraphicsView::paintEvent(event);
    QPainter painter(this->viewport());
    if(image.isNull()){
        painter.setPen(qApp->palette().light().color());
        painter.drawText(rect(), Qt::AlignCenter, tr("CTRL + O - Open image\nCTRL + Q - Exit"));
    }
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier)
    {
        const ViewportAnchor archor = transformationAnchor();
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        int angle = event->angleDelta().y();
        double factor;
        if(angle > 0)
        {
            factor = 1.1;
        }
        else
        {
            factor = 0.9;
        }
        scale(factor, factor);
        setTransformationAnchor(archor);
        emit scaleChanged(factor);
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}
