#ifndef CHARACTER_H
#define CHARACTER_H

#include <map>
#include <cmath>
#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <vector>
#include "Wall.h"


enum class Name {PENGUIN_WELDER};

enum class Movement{UP, LEFT, DOWN, RIGHT, IDLE_UP, IDLE_LEFT, IDLE_RIGHT, IDLE_DOWN};



class Character : public QObject, public QGraphicsItem
{
    Q_OBJECT
private:
    int currentFrameX;
    int currentFrameY;
    QPixmap * sprite;
    QTimer * animationTimer;
    QTimer * movementTimer;
    int size;
    Movement movement;
    bool step;
    float speed;
    int bombs;
public:
    explicit Character(QObject *parent = 0);
    explicit Character(Name name, float x = 0, float y = 0, int size = 64, QObject *parent = 0);
    void setMovement(Movement mov);
    Movement getMovement();
    QRectF getBounds();
    //dodanie bomb, ktore moze rzucic
    void setBombs(int b);
    int getBombs();
    bool bombCollision(float bombX, float bombY, float radius);
public slots:
    void move(std::vector<Wall*> * walls);


private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget);
    QRectF boundingRect() const;
    void nextFrame();
    void calcFrame(int frameY, bool idle);
    bool collision(Wall * wall, float speedX, float speedY);
    bool allCollisions(std::vector<Wall*> * walls, float speedX, float speedY);


};

#endif // CHARACTER_H
