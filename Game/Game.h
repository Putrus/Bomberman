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
#include "Object.h"
class Game : public QGraphicsView
{
public:
    Game(int players, QWidget * parent = 0);
private:
    QGraphicsScene *scene;
    QTimer * timer;
    QTimer * animationTimer;
    std::vector<bmb::Object*> * objects;
    std::vector<bmb::Character*> * characters;
private:
    void updateGame();
    void updateAnimation();
    bool bombCollision(bmb::Object * object, bmb::Bomb * bomb);
    bool collision(bmb::Character * character, bmb::Object * object);
    bool allBombsCollision(bmb::Object * object, std::vector<bmb::Object*> * objects);
    bool allCollisions(bmb::Character * character, std::vector<bmb::Object*> * objects);
    std::vector<bmb::Character*> * getCharacter();
};

#endif // GAME_H
