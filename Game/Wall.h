#ifndef WALL_H
#define WALL_H
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include "Object.h"
namespace bmb
{
class Wall : public Object
{
    Q_OBJECT
public:
    Wall(QString name, QPointF pos = QPointF(0.f, 0.f), QRectF bounds = QRectF(0.f, 0.f, 0.f, 0.f), bool isBreakable = false, Object *parent = 0);
private:
    bool isBreakable;
public:
    void nextFrame();
    void gameUpdate(bool isDamage = false, bool canMove = true);
};
}
#endif // WALL_H
