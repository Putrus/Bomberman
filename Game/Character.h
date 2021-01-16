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
#include "Object.h"




namespace bmb{
class Character : public Object
{
private:
    bool step;
    //aktualna predkosc
    QPointF speed;
    //predkosc ogolna maksymalna
    float velocity;
    int bombs;
    //to jeszcze moze do zmienienia i wrzucenia do klasy Player
    int score;
private:
    void calcFrame(float frameY, bool idle);
public:
    explicit Character(QString name, QPointF pos = QPointF(0.f, 0.f), QRectF bounds = QRectF(0.f, 0.f, 0.f, 0.f), Object *parent = 0);
    void nextFrame();
    void gameUpdate(bool isDamage = false, bool canMove = true);

    //jak setAnimation tylko jeszcze predkosc sie ustala automatycznie
    void setAction(Animation action);
    //getters and setters
    int getBombs();
    void setBombs(int bombs);

    QPointF getSpeed();
    void setSpeed(QPointF speed);
    void setSpeed(float speedX, float speedY);

    float getVelocity();
    void setVelocity(float velocity);

    int getScore();
    void setScore(int score);
    void addScore(int add);


};
}
#endif // CHARACTER_H
