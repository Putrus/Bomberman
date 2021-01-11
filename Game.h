#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QDebug>
#include <QMouseEvent>
#include <QObject>
#include "Bomb.h"
#include "Character.h"
#include "Wall.h"
class Game : public QGraphicsView
{
public:
    Game(QWidget * parent = 0);



private:
    QGraphicsScene *scene;
    QTimer *timer;
    Character * character;
    std::vector<Wall*> * walls;
    std::vector<Bomb*> * bombs;

private:
    void mousePressEvent(QMouseEvent *ev);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void checkBombCollisions();
};

#endif // GAME_H