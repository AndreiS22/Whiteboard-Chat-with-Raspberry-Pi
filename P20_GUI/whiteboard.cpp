#include "whiteboard.h"
#include <QMouseEvent>
#include <QPainter>
#include <QColorDialog>
#include <QInputDialog>
#include <QDesktopServices>
#include <QDebug>
#include <QMutex>
#include <QVector>

Whiteboard::Whiteboard(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StyledBackground);

    // Setting default whiteboard settings
    backgroundColour = QColor("White");
    penColour = QColor("Black");
    penSize = 10;

    // Setting up whiteboard for drawing
    QImage blank(QSize(612, 475), QImage::Format_RGB32);
    blank.fill(backgroundColour);
    display = blank;
    update();
}

void Whiteboard::checkVector(){
    receiveMutex->lock();
    commandVector.push_back(receiveVector[0]); //Get the data from the receiver
    receiveVector.erase(receiveVector.begin());
    receiveMutex->unlock();
    //Interpret the data from the receiver
    ///Numbers are being passed one by one, so we have to wait for all data to be sent before doing anything
    if(commandVector[0] == 1 && commandVector.size() == 5){
        qDebug() << commandVector[1] << commandVector[2] << commandVector[3] << commandVector[4];
        drawReceivedLine(QPoint(commandVector[1], commandVector[2]), QPoint(commandVector[3], commandVector[4]));
        commandVector.clear();
    } else if(commandVector[0] == 2){
        clearDrawArea();
        commandVector.clear();
    } else if(commandVector[0] == 3 && commandVector.size() == 4){
        setReceivedBackgroundColour(QColor(commandVector[1], commandVector[2], commandVector[3]));
        commandVector.clear();
    } else if(commandVector[0] == 4 && commandVector.size() == 4){
        setReceivedPenColour(QColor(commandVector[1], commandVector[2], commandVector[3]));
        commandVector.clear();
    } else if(commandVector[0] == 5 && commandVector.size() == 2){
        setReceivedPenSize(commandVector[1]);
        commandVector.clear();
    } else if(commandVector[0] == 6){
        QDesktopServices::openUrl(QUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"));
        commandVector.clear();
    }
}

void Whiteboard::sendLeFunny() {
    transmitMutex->lock();
    transmitVector.push_back(6);
    transmitMutex->unlock();
}

void Whiteboard::configureMutexPointers(QMutex *transmitMutexPtr, QMutex *receiveMutexPtr) {
    transmitMutex = transmitMutexPtr;
    receiveMutex = receiveMutexPtr;
}

//The following pairs of functions change the settings of drawArea and displayArea

//1
void Whiteboard::setBackgroundColour() {
    QColor newColour = QColorDialog::getColor(backgroundColour);
    if(newColour.isValid()) {
        backgroundColour = newColour;
        transmitMutex->lock();
        transmitVector.push_back(3);
        transmitVector.push_back(backgroundColour.red());
        transmitVector.push_back(backgroundColour.green());
        transmitVector.push_back(backgroundColour.blue());
        transmitMutex->unlock();
        qDebug() << "Action:setBackgroundColour, Colour:" << backgroundColour;
        clearDrawArea();
    }
}

void Whiteboard::setReceivedBackgroundColour(QColor colour) {
    backgroundColour = colour;
    clearDrawArea();
}

//2

void Whiteboard::setPenColour() {
    QColor newColour = QColorDialog::getColor(penColour);
    if(newColour.isValid()) {
        penColour = newColour;
        transmitMutex->lock();
        transmitVector.push_back(4);
        transmitVector.push_back(penColour.red());
        transmitVector.push_back(penColour.green());
        transmitVector.push_back(penColour.blue());
        transmitMutex->unlock();
        qDebug() << "Action:setPenColour, Colour:" << penColour;
    }
}

void Whiteboard::setReceivedPenColour(QColor colour) {
    penColour = colour;
}

//3

void Whiteboard::setPenSize() {
    penSize = QInputDialog::getInt(this, "Pen Size", "Select pen size:", penSize, 1, 50, 1);
    transmitMutex->lock();
    transmitVector.push_back(5);
    transmitVector.push_back(penSize);
    transmitMutex->unlock();
    qDebug() << "Action:setPenSize, Value:" << penSize;
}

void Whiteboard::setReceivedPenSize(int size) {
    penSize = size;
}

//Register the coordinates of the cursor as it's moving, from when it's clicked until it's released

void Whiteboard::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton && drawEnable) {
        prevPoint = QPoint(event->pos().x(), event->pos().y());
    }
}

void Whiteboard::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton && drawEnable) {
        drawLine(event->pos());
    }
}

void Whiteboard::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton && drawEnable) {
        drawLine(event->pos());
    }
}

void Whiteboard::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect = event->rect();
    painter.drawImage(rect, display, rect);
}

//4
//These implement the actual drawing

void Whiteboard::drawLine(const QPoint &point) {
    QPainter painter(&display);
    painter.setPen(QPen(penColour, penSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(prevPoint, point);
    // Transmit coords here
    transmitMutex->lock();
    transmitVector.push_back(1);
    transmitVector.push_back(prevPoint.x());
    transmitVector.push_back(prevPoint.y());
    transmitVector.push_back(point.x());
    transmitVector.push_back(point.y());
    transmitMutex->unlock();
    qDebug() << "Action:drawLine, From:(" << prevPoint.x() << "," << prevPoint.y() << ") to (" << point.x() << "," << point.y() << ")";
    update();
    prevPoint = point;
}

void Whiteboard::drawReceivedLine(const QPoint &prevPoint, const QPoint &point) {
    QPainter painter(&display);
    painter.setPen(QPen(penColour, penSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(prevPoint, point);
    update();
}

void Whiteboard::clearDrawArea() {
    display.fill(backgroundColour);
    qDebug() << "Action:clearDrawArea";
    transmitMutex->lock();
    transmitVector.push_back(2);
    transmitMutex->unlock();
    update();
}

/* Command types:
 * -1 = reserved for no command available in getCommand
 * 1 = drawLine
 * 2 = clearDrawArea
 * 3 = setBackgroundColour
 * 4 = setPenColour
 * 5 = setPenSize
 * 6 = Le Funny
*/
