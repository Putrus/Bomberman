#ifndef BOMB_H
#define BOMB_H
#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>
#include "Object.h"
#include "Character.h"
namespace bmb{
class Bomb : public Object
{
private:
    //postac do ktorej nalezy bomba
    Character * character;
    float explosionRadius;
public:
    explicit Bomb(Character & character, QString name = "bomb", QPointF pos = QPointF(-420.69f, -420.69f), QRectF bounds = QRectF(0.f, 0.f, 0.f, 0.f), Object *parent = 0);
    bool ifExplodes();
    void nextFrame();
    void gameUpdate(bool isDamage = false, bool canMove = true);
    //getters and setters
    float getExplosionRadius();
    void setExplosionRadius(float radius);

    Character * getCharacter();
    void setCharacter(Character & character);
};
}
#endif // BOMB_H
