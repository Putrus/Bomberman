#ifndef BOMBERMAN_H
#define BOMBERMAN_H

#include <QMainWindow>
#include <QFontDatabase>
#include <QFont>
#include <QDebug>
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
private:
    void showMenu(QWidget * widgetShow = nullptr, QWidget * widgetHide = nullptr);
    void hideAll();
    void buttonConnects();


    //getters setters
    void setUsername(QString username);
    void readUsername();
};

#endif // BOMBERMAN_H
