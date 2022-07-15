#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "whiteboard.h"
#include <QMutex>
#include <QVector>

class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT //Qt macro indicating the class should be run through the "Meta-Object Compiler" (moc)

public:
    explicit MainWindow(QWidget *parent = nullptr, QMutex *transmitMutexPtr = nullptr, QMutex *receiveMutex = nullptr); //Constructor
    int getCommand();
    void setCommand(int a);
    QMutex *transmitMutex, *receiveMutex;

public slots:
    void goToChecking();

private slots:
    void on_clearButton_clicked();
    void on_funnyButton_clicked();
    void on_sizeButton_clicked();
    void on_penColourButton_clicked();
    void on_bgColourButton_clicked();
};
#endif // MAINWINDOW_H
