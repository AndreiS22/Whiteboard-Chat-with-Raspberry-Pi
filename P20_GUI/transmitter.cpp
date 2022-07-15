#include "transmitter.h"
#include "mainwindow.h"
#include <QDebug>
#include "wiringPi.h"

Transmitter::Transmitter(QWidget *parent, MainWindow* windowPtr) {
    localPtr = windowPtr; //Pointer used to interface with the main window
}

void Transmitter::run() {
    while(1) {
        int c = localPtr->getCommand();
        if(c > -1) { // Check if we have data to send over GPIO
            qDebug() << "Decimal: " << c;

            int decimal = c; //Converting from decimal to binary
            bool binary[10];
            for(int i=9; i>=0; i--) {
                binary[i] = decimal % 2;
                decimal /= 2;
            }

            digitalWrite(15, 1); // Start bit
            //qDebug() << "Start bit";

            // Read request
            digitalWrite(8, 1);
            while(!digitalRead(9));
            digitalWrite(8, 0);
            while(digitalRead(9));

            for(int i=0; i<10; i++) {
                digitalWrite(15, binary[i]);

                // Read request
                digitalWrite(8, 1);
                qDebug() << binary[i];
                //delay(10);
                while(!digitalRead(9));
                digitalWrite(8, 0);
                while(digitalRead(9));
            }
            digitalWrite(15, 0);
        }
    }
}

