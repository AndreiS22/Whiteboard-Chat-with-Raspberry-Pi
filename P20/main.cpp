#include "mainwindow.h"
#include "sendwindow.h"
#include "receivewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SendWindow w1;
    ReceiveWindow w2;
    w1.show();
    w2.show();
    return a.exec();
}
