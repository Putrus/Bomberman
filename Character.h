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

enum class Movement{UP, LEFT, DOWN, RIGHT, IDLE_UP, IDLE_LEFT, IDLE_RIGHT, IDLE_DOWN, DEAD};



class Character : public QObject, public QGraphicsItem
{
    Q_OBJECT
private:
    int currentFrameX;
    int currentFrameY;
    QPixmap * sprite;
    QTimer * animationTimer;
    QTimer * movementTimer;
    //wielkosc obrazka postaci
    int size;
    //jaka czynnosc wykonuje
    Movement movement;
    //step - zmienna do animacji ruchu(czy jest na lewej czy prawej nodze gdy idzie) poniewaz tak zbudowany jest sprite
    bool step;
    float speed;
    //ilosc bomb jakie moze rzucic w danym momencie
    int bombs;
    //czy zyje
    bool isAlive;
public:
    explicit Character(QObject *parent = 0);
    explicit Character(Name name, float x = 0, float y = 0, int size = 64, QObject *parent = 0);
    void setMovement(Movement mov);
    Movement getMovement();
    QRectF getBounds();
    //kolizje z bombami
    bool bombCollision(float bombX, float bombY, float radius);
    //getters and setters
    //dodanie bomb, ktore moze rzucic
    void setBombs(int b);
    int getBombs();
    //sprawdzenie czy zyje
    bool getIsAlive();
    //ustawienie nowej predkosci
    void setSpeed(float v);
    float getSpeed();
    int score;

public slots:
    void move(std::vector<bmb::Wall*> * walls);


private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget);
    QRectF boundingRect() const;
    void nextFrame();
    void calcFrame(int frameY, bool idle);
    bool collision(bmb::Wall * wall, float speedX, float speedY);
    bool allCollisions(std::vector<bmb::Wall*> * walls, float speedX, float speedY);


};

#endif // CHARACTER_H
