#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QWidget>
#include <QImage>
#include <QMouseEvent>
#include <QVector>
#include <QMutex>

class Whiteboard : public QWidget {
    Q_OBJECT

public:
    Whiteboard(QWidget *parent = nullptr);
    bool drawEnable; // Toggles whether you can draw on the widget
    bool receiveEnable; // Toggles whether received commands will be drawn on the widget
    void clearDrawArea(); // Clears the draw area (fill with selected background colour)
    void setBackgroundColour();
    void setPenColour();
    void setPenSize();
    //Change the settings in the receiver area when it gets the commands from the transmitter
    void setReceivedBackgroundColour(QColor colour);
    void setReceivedPenColour(QColor colour);
    void setReceivedPenSize(int size);
    QVector<int> transmitVector, receiveVector, commandVector; //used to exchange data between the receiver, transmitter and the whiteboard
    void configureMutexPointers(QMutex *transmitMutexPtr, QMutex *receiveMutexPtr);
    void sendLeFunny();

public slots:
    void checkVector();


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    // Whiteboard settings
    QColor backgroundColour;
    QColor penColour;
    int penSize;


    void drawLine(const QPoint &point);
    void drawReceivedLine(const QPoint &prevPoint, const QPoint &point);
    QPoint prevPoint;
    QImage display;
    QMutex *transmitMutex, *receiveMutex;
};

#endif // WHITEBOARD_H
