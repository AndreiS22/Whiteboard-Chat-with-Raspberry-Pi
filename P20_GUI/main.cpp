#include <QApplication>
#include <QDebug>
#include <QVector>
#include <QMutex>

#include <pthread.h>
#include <wiringPi.h>

#include "mainwindow.h"
#include "transmitter.h"
#include "receiver.h"

int main(int argc, char *argv[]) {
    // setup Qt GUI and threads
    QApplication a(argc, argv);

    QMutex *transmitMutex = new QMutex;
    QMutex *receiveMutex = new QMutex;

    MainWindow *wPtr = new MainWindow(nullptr, transmitMutex, receiveMutex);

    Transmitter *tPtr = new Transmitter(nullptr, wPtr);
    Receiver *rPtr = new Receiver(nullptr, wPtr);

    //Connect the signal from the receiver to the slot from displayArea
    QObject::connect(rPtr, SIGNAL(vectorUpdated()), wPtr, SLOT(goToChecking()));


    wPtr->show();
    tPtr->start();
    rPtr->start();

    wiringPiSetup();
    pinMode(15, OUTPUT); // Transmitting Data
    pinMode(8, OUTPUT); // Transmitting Read
    pinMode(9, INPUT); // Transmitting Confirm

    pinMode(16, INPUT); // Receiving Data
    pinMode(2, INPUT); // Receiving Read
    pinMode(3, OUTPUT); // Receiving Confirm

    ///Wiring explanation
    /// TData: 15->16       RData: 15<-16
    /// TRead: 8->2         RRead: 8<-2
    /// TConfirm: 9->3      RConfirm: 9<-3


    // start window event loop
    qDebug() << "Starting event loop...";
    int ret = a.exec();
    qDebug() << "Event loop stopped.";

    // cleanup pthreads
    //pthread_exit(NULL);

    // exit
    return ret;
}

