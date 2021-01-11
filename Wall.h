#ifndef WALL_H
#define WALL_H
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
class Wall : public QObject, public QGraphicsItem
{
    Q_OBJECT
private:
    QPixmap * sprite;
public:
    Wall(QObject *parent=0);
    Wall(float x, float y, QObject *parent = 0);
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget);
    QRectF boundingRect() const;
public:
    QRectF getBounds();
};

#endif // WALL_H
