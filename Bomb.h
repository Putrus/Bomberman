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
    Q_OBJECT
private:
    //postac do ktorej nalezy bomba
    Character * character;
public:
    explicit Bomb(Character & character, QString name = "bomb", QPointF pos = QPointF(-420.69f, -420.69f), QRectF bounds = QRectF(0.f, 0.f, 0.f, 0.f), Object *parent = 0);
    bool ifExplodes();
private:
    float explosionRadius;
public:
    void nextFrame();
};
}
#endif // BOMB_H
