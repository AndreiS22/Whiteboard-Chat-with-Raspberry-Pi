#include "receiver.h"
#include <QDebug>
#include <wiringPi.h>

Receiver::Receiver(QWidget *parent, MainWindow* winPtr)
{
    localPtr = winPtr; //Pointer used to interface with the main window
}

int Receiver::bin2dec(){ //Convert from binary to decimal
    while(!digitalRead(2));
    digitalRead(16);
    digitalWrite(3, 1);
    while(digitalRead(2));
    digitalWrite(3, 0);
    int x = 0, exp = 512;
    for(int i = 1; i <= 10; ++i){
        while(!digitalRead(2));
        x += exp * digitalRead(16);
        exp /= 2;
        digitalWrite(3, 1);
        while(digitalRead(2));
        digitalWrite(3, 0);
    }
    return x;
}

void vectorUpdated(); //Signal used to indicate that something's been sent over the GPIO pins

void Receiver::run(){ //Receiver thread action
    while(1){
        int c = bin2dec();
        localPtr->setCommand(c); //Sends the number from the GPIO to the whiteboard
        vectorUpdated();
    }
}
