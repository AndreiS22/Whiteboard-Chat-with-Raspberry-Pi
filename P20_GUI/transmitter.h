#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include "mainwindow.h"
#include <QThread>
#include <QMutex>

class Transmitter : public QThread {
    Q_OBJECT
public:
    Transmitter(QWidget *parent = nullptr, MainWindow *windowPtr = nullptr);
private:
    void run();
    MainWindow* localPtr;
};

#endif // TRANSMITTER_H
