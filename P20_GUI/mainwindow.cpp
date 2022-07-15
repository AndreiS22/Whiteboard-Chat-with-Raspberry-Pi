#include "mainwindow.h"
#include <QMouseEvent>
#include <QDebug>
#include <QMutex>
#include <wiringPi.h>

MainWindow::MainWindow(QWidget *parent, QMutex *transmitMutexPtr, QMutex *receiveMutexPtr) : QMainWindow(parent) {
    setupUi(this); //Loads the .ui file
    //allow drawArea to receive inputs and block displayArea
    drawArea->drawEnable = true;
    drawArea->receiveEnable = false;
    displayArea->drawEnable = false;
    displayArea->receiveEnable = true;

    transmitMutex = transmitMutexPtr;
    receiveMutex = receiveMutexPtr;

    drawArea->configureMutexPointers(transmitMutex, receiveMutex);
    displayArea->configureMutexPointers(transmitMutex, receiveMutex);

}

void MainWindow::goToChecking(){
    displayArea->checkVector();
}

int MainWindow::getCommand() {
    if(drawArea->transmitVector.empty()) {
        return -1;
    }
    transmitMutex->lock();
    int commandCopy = drawArea->transmitVector[0];
    drawArea->transmitVector.erase(drawArea->transmitVector.begin());
    transmitMutex->unlock();
    return commandCopy;
}

void MainWindow::setCommand(int data) {
    receiveMutex->lock();
    displayArea->receiveVector.push_back(data);
    receiveMutex->unlock();
}


void MainWindow::on_sizeButton_clicked() {
    drawArea->setPenSize();
}

void MainWindow::on_penColourButton_clicked() {
    drawArea->setPenColour();
}

void MainWindow::on_bgColourButton_clicked() {
    drawArea->setBackgroundColour();
}

void MainWindow::on_clearButton_clicked() {
    drawArea->clearDrawArea();
}

void MainWindow::on_funnyButton_clicked() {
    //QDesktopServices::openUrl(QUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"));
    //qDebug() << "Action:leFunny";
    drawArea->sendLeFunny();
}
