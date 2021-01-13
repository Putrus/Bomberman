#include "Object.h"

bmb::Object::Object(QString name, QPointF pos, QRectF bounds, QObject * parent) : QObject(parent) , QGraphicsItem()
{
    QDir dir("images");
    if(!dir.exists(name + ".png"))
    {
        qDebug() << "Failed to load file " + name + ".png. Create Object is impossible!";
    }
    else{
        sprite = new QPixmap("images/" + name + ".png");
        if(name == "bomb_sprite")
        {
            spriteSize = 128.f;
        }
        else{
            spriteSize = 64.f;
        }
        if(bounds == QRectF(0.f, 0.f, 0.f, 0.f))
        {
            this->bounds = QRectF(0.f, 0.f, spriteSize, spriteSize);
        }
        else{
            this->bounds = bounds;
        }
        currentFrame = QPointF(0.f,0.f);
        toDeleted = false;
        setPos(pos);
        animation = bmb::Animation::NO;
    }
}





void bmb::Object::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget)
{
    painter->drawPixmap(0, 0, *sprite, currentFrame.x(), currentFrame.y(), spriteSize, spriteSize);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}


QRectF bmb::Object::boundingRect() const
{
    return QRectF(0, 0, spriteSize, spriteSize);
}

void bmb::Object::nextFrame()
{}

void bmb::Object::gameUpdate(bool isDamage, bool canMove)
{}


QRectF bmb::Object::getBounds()
{

    return QRectF(pos().x() + bounds.left(), pos().y() + bounds.top(), bounds.width(), bounds.height());
}

void bmb::Object::setBounds()
{

}

bool bmb::Object::getToDeleted()
{
    return toDeleted;
}
void bmb::Object::setToDeleted(bool d)
{
    toDeleted = d;
}

QPointF bmb::Object::getCurrentFrame()
{
    return currentFrame;
}
void bmb::Object::setCurrentFrame(QPointF frame)
{
    currentFrame = frame;
}

void bmb::Object::setCurrentFrame(float x, float y)
{
    currentFrame = QPointF(x,y);
}

float bmb::Object::getSpriteSize()
{
    return spriteSize;
}

void bmb::Object::setSpriteSize(float size)
{
    spriteSize = size;
}

bmb::Animation bmb::Object::getAnimation()
{
    return animation;
}

void bmb::Object::setAnimation(Animation animation)
{
    this->animation = animation;
}
