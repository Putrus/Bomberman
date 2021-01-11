#ifndef BOMB_H
#define BOMB_H
#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>
#include "Character.h"
class Bomb : public QObject, public QGraphicsItem
{
    Q_OBJECT
private:
    //postac do ktorej nalezy bomba
    Character * character;
public:
    explicit Bomb(QObject *parent = 0);
    explicit Bomb(Character & character, float x, float y,QObject *parent = 0);
    bool ifExplodes();
    bool ifToDeleted();
private slots:
    void nextFrame();

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget);
    QRectF boundingRect() const;
private:
    QTimer *animationTimer;
    QPixmap *spriteImage;
    int currentFrameX;
    int currentFrameY;
    float explosionRadius;
    bool toDeleted;

};

#endif // BOMB_H
