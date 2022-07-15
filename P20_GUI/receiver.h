#ifndef RECEIVER_H
#define RECEIVER_H

#include "mainwindow.h"
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <wiringPi.h>

class Receiver : public QThread
{
    Q_OBJECT
public:
    Receiver(QWidget *parent = nullptr, MainWindow *winPtr = nullptr);
    int bin2dec();

signals:
    void vectorUpdated();

private:
    void run();
    MainWindow* localPtr;
};

#endif // RECEIVER_H
