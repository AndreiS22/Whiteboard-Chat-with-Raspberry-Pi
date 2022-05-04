#include <QtWidgets>

#include "sendwindow.h"
#include "sendboard.h"

SendWindow::SendWindow(QWidget *parent)
    : QMainWindow{parent}
{
    sendBoard = new SendBoard;
    setCentralWidget(sendBoard);

    createActions();
    createMenus();

    setWindowTitle("Send");

    resize(500, 500);
}

void SendWindow::closeEvent(QCloseEvent *event){
    if(maybeSave()){
        event->accept();
    } else{
        event->ignore();
    }
}


void SendWindow::save(){
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void SendWindow::penColor(){
    QColor newColor = QColorDialog::getColor(sendBoard->penColor());
    if(newColor.isValid()){
        sendBoard->setPenColor(newColor);
    }
}

void SendWindow::penWidth(){
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Send"), tr("Select pen width: "), sendBoard->penWidth(), 1, 50, 1, &ok);
    if(ok){
        sendBoard->setPenWidth(newWidth);
    }
}


void SendWindow::createActions(){
    foreach(QByteArray format, QImageWriter::supportedImageFormats()){
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);

    }

    exitAct = new QAction(tr("&Exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("&Pen Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction("&Clear Screen...", this);
    connect(clearScreenAct, SIGNAL(triggered()), sendBoard, SLOT(clearImage()));
}

void SendWindow::createMenus(){
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach(QAction *action, saveAsActs){
        saveAsMenu->addAction(action);
    }
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addAction(clearScreenAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
}

bool SendWindow::maybeSave(){
    if(sendBoard->isModified()){
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Send"), tr("The image has been modified.\n Do you want to save changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
       if(ret == QMessageBox::Save){
           return saveFile("png");
       } else if(ret == QMessageBox::Cancel){
           return false;
       }
    }
    return true;
}

bool SendWindow::saveFile(const QByteArray &fileFormat){
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"), initialPath, tr("%1 Files (*.%2);; All Files (*)").arg(QString::fromLatin1(fileFormat.toUpper())).arg(QString::fromLatin1(fileFormat)));

    if(fileName.isEmpty()){
        return false;
    } else{
        return sendBoard->saveImage(fileName, fileFormat.constData());
    }
}
