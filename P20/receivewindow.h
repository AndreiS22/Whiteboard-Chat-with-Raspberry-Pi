#ifndef RECEIVEWINDOW_H
#define RECEIVEWINDOW_H

#include <QList>
#include <QMainWindow>

class ReceiveBoard;

class ReceiveWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ReceiveWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;


private slots:
    void save();
    void penColor();
    void penWidth();


private:
    void createActions();
    void createMenus();

    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    ReceiveBoard *receiveBoard;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;

    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *clearScreenAct;

    QString name;


};

#endif // RECEIVEWINDOW_H
