#include "Bomb.h"

Bomb::Bomb(QObject *parent) : QObject(parent), QGraphicsItem()
{}

Bomb::Bomb(Character & character, float x, float y, QObject *parent) : QObject(parent), QGraphicsItem()
{
    currentFrameX = 0;
    currentFrameY = 0;
    spriteImage = new QPixmap("images/bomb_sprite.png");
    animationTimer = new QTimer();
    connect(animationTimer, &QTimer::timeout, this, &Bomb::nextFrame);
    animationTimer->start(100);
    setPos(x,y);
    this->character = &character;
    this->character->setBombs(this->character->getBombs()-1);
    explosionRadius = 16;
}



void Bomb::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget)
{
    painter->drawPixmap(0,0,*spriteImage, currentFrameX, currentFrameY, 128, 128);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF Bomb::boundingRect() const
{
    return QRectF(0,0,128,128);
}


void Bomb::nextFrame()
{
    currentFrameX+=128;
    if(currentFrameX == 1152 && currentFrameY == 128)
    {
        character->setBombs(character->getBombs()+1);
        delete this;
        return;
    }
    if(currentFrameX == 1152)
    {
        currentFrameX = 0;
        currentFrameY += 128;
        setZValue(6);
    }
    this->update(0,0,128,128);
}

bool Bomb::ifExplodes()
{
    if(currentFrameY == 128)
    {
        return true;
    }
    return false;
}

