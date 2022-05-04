#include <QtWidgets>

#include "receiveboard.h"

ReceiveBoard::ReceiveBoard(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);

    modified = false;
    scribbling = false;
    myPenWidth = 3;
    myPenColor = Qt::black;

}

bool ReceiveBoard::saveImage(const QString &fileName, const char *fileFormat){
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());
    if(visibleImage.save(fileName, fileFormat)){
        modified = false;
        return true;
    }
    return false;
}

void ReceiveBoard::setPenColor(const QColor &newColor){
    myPenColor = newColor;
}

void ReceiveBoard::setPenWidth(int newWidth){
    myPenWidth = newWidth;
}

void ReceiveBoard::clearImage(){
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}

void ReceiveBoard::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
        scribbling = true;
    }
}

void ReceiveBoard::mouseMoveEvent(QMouseEvent *event){
    if((event->buttons() & Qt::LeftButton) && scribbling){
        drawLineTo(event->pos());
    }
}

void ReceiveBoard::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && scribbling){
        drawLineTo(event->pos());
        scribbling = false;
    }
}

void ReceiveBoard::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QRect canvas = event->rect();
    painter.drawImage(canvas, image, canvas);
}

void ReceiveBoard::resizeEvent(QResizeEvent *event){
    if(width() > image.width() || height() > image.height()){
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void ReceiveBoard::drawLineTo(const QPoint &endPoint){
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void ReceiveBoard::resizeImage(QImage *image, const QSize &newSize){
    if(image->size() == newSize){
        return;
    }

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255,255,255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

