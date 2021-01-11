#include "Wall.h"
Wall::Wall(QObject *parent) : QObject(parent), QGraphicsItem()
{

}
Wall::Wall(float x, float y, QObject *parent) : QObject(parent), QGraphicsItem()
{


    sprite = new QPixmap("images/wall.png");
    setPos(x,y);
}





void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget)
{
    painter->drawPixmap(0, 0 , *sprite, 0, 0, 34, 34);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}


QRectF Wall::boundingRect() const
{
    return QRectF(0,0,34,34);
}

QRectF Wall::getBounds()
{
    QRectF rect(pos().x(),pos().y(),34,34);
    return rect;
}

