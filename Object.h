#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QDir>


namespace bmb
{
enum class Animation {NO, IDLE_UP, IDLE_DOWN, IDLE_LEFT, IDLE_RIGHT, UP, DOWN, LEFT, RIGHT, BREAK
};

class Object : public QObject, public QGraphicsItem
{
    Q_OBJECT
private:
    QPointF currentFrame;
    QPixmap *sprite;
    float spriteSize;
    QRectF bounds;
    bool toDeleted;
    Animation animation;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
public:
    Object(QString name, QPointF pos = QPointF(0.f, 0.f), QRectF bounds = QRectF(0.f, 0.f, 0.f, 0.f), QObject *parent = 0);
    virtual void nextFrame();

    //getters and setters
    QRectF getBounds();
    void setBounds();

    bool getToDeleted();
    void setToDeleted(bool d);

    QPointF getCurrentFrame();

    void setCurrentFrame(QPointF frame);
    void setCurrentFrame(float x, float y);

    float getSpriteSize();
    void setSpriteSize(float size);


    Animation getAnimation();
    void setAnimation(Animation animation);
};
}
#endif // GAMEOBJECT_H
