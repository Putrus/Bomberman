#ifndef BOMBERMAN_H
#define BOMBERMAN_H

#include <QMainWindow>
#include <QFontDatabase>
#include <QFont>
#include <QDebug>
#include <QTcpSocket>
#include <QMessageBox>
#include "Game.h"


namespace Ui {
class Bomberman;
}

class Bomberman : public QMainWindow
{
    Q_OBJECT
public:
    explicit Bomberman(QWidget *parent = nullptr);
    ~Bomberman();

private:
    Ui::Bomberman *ui;
    Game * game;
    QString username;
    QTcpSocket *sock;
    QTimer * connTimeoutTimer;
private:
    void showMenu(QWidget * widgetShow = nullptr, QWidget * widgetHide = nullptr);
    void hideAll();
    void buttonConnects();
    //sieciowe sprawy
    void connectBtnHit();
    void joinBtnHit();
    void socketConnected();
    void socketDisconnected();
    void socketReadable();
    //getters setters
    void setUsername(QString username);
    void readUsername();
    void sendMessage(QString message);
    void createRoom();
};

#endif // BOMBERMAN_H
