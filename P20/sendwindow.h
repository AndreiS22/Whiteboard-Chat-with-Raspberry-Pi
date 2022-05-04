#ifndef SENDWINDOW_H
#define SENDWINDOW_H

#include <QList>
#include <QMainWindow>

class SendBoard;

class SendWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SendWindow(QWidget *parent = nullptr);

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

    SendBoard *sendBoard;

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

#endif // SENDWINDOW_H
