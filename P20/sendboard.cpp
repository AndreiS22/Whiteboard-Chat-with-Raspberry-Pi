#include <QtWidgets>

#include "sendboard.h"

SendBoard::SendBoard(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);

    modified = false;
    scribbling = false;
    myPenWidth = 3;
    myPenColor = Qt::red;

}

bool SendBoard::saveImage(const QString &fileName, const char *fileFormat){
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());
    if(visibleImage.save(fileName, fileFormat)){
        modified = false;
        return true;
    }
    return false;
}

void SendBoard::setPenColor(const QColor &newColor){
    myPenColor = newColor;
}

void SendBoard::setPenWidth(int newWidth){
    myPenWidth = newWidth;
}

void SendBoard::clearImage(){
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}

void SendBoard::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
        scribbling = true;
    }
}

void SendBoard::mouseMoveEvent(QMouseEvent *event){
    if((event->buttons() & Qt::LeftButton) && scribbling){
        drawLineTo(event->pos());
    }
}

void SendBoard::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && scribbling){
        drawLineTo(event->pos());
        scribbling = false;
    }
}

void SendBoard::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QRect canvas = event->rect();
    painter.drawImage(canvas, image, canvas);
}

void SendBoard::resizeEvent(QResizeEvent *event){
    if(width() > image.width() || height() > image.height()){
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void SendBoard::drawLineTo(const QPoint &endPoint){
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void SendBoard::resizeImage(QImage *image, const QSize &newSize){
    if(image->size() == newSize){
        return;
    }

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255,255,255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

