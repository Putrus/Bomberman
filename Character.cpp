#include "Character.h"

Character::Character(QObject *parent) :QObject(parent), QGraphicsItem()
{

}
Character::Character(Name name, float x, float y, int size, QObject *parent) : QObject(parent), QGraphicsItem()
{
    QString n = "images/";
    switch(name)
    {
        case Name::PENGUIN_WELDER:
            n += "penguin_welder.png";
            break;
        default:
            n += "penguin_welder.png";
            break;
    }
    sprite = new QPixmap(n);
    this->size = size;
    currentFrameX = 0;
    currentFrameY = 0;
    animationTimer = new QTimer();
    connect(animationTimer, &QTimer::timeout, this, &Character::nextFrame);
    animationTimer->start(100);
    step = 0;
    speed = 2;
    movement = Movement::IDLE_DOWN;
    bombs = 1;
    isAlive = true;
    score = 0;
}

void Character::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget)
{
    painter->drawPixmap(0, 0 , *sprite, currentFrameX, currentFrameY, size, size);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}


QRectF Character::boundingRect() const
{
    return QRectF(0,0,size,size);
}

void Character::nextFrame()
{
    switch(movement)
    {
    case Movement::IDLE_UP:
        calcFrame(size*2, true);
        break;
    case Movement::IDLE_LEFT:
        calcFrame(size*3, true);
        break;
    case Movement::IDLE_DOWN:
        calcFrame(0, true);
        break;
    case Movement::IDLE_RIGHT:
        calcFrame(size, true);
        break;
    case Movement::UP:
        calcFrame(size*6, false);
        break;
    case Movement::LEFT:
        calcFrame(size*7, false);
        break;
    case Movement::DOWN:
        calcFrame(size*4, false);
        break;
    case Movement::RIGHT:
        calcFrame(size*5, false);
        break;
    case Movement::DEAD:
        calcFrame(size*8,true);
        break;
    }
    this->update(0,0,size,size);
}

void Character::calcFrame(int frameY, bool idle)
{
    //czy postac zyje
    if(frameY != size*8)
    {
    if(idle == true)
    {
        currentFrameY = frameY;
        if(currentFrameX == size)
        {
            currentFrameX = 0;
        }
        else{
            currentFrameX = size;
        }
    }
    if(idle == false)
    {
        if(currentFrameY == frameY - size*4)
        {
            currentFrameY = frameY;
            if(step == 0)
            {
                currentFrameX = 0;
                step = 1;
            }else{
                currentFrameX = size;
                step = 0;
            }
        }
        else{
            currentFrameY = frameY - size*4;
            currentFrameX = 0;
        }
    }
    }
    else{
        isAlive = false;
        currentFrameY = frameY;
        if(currentFrameX != 128)
        {
            currentFrameX +=64;
        }
    }
}
void Character::move(std::vector<bmb::Wall*> * walls)
{
    switch(movement)
    {
        case Movement::RIGHT:
            if(!allCollisions(walls, speed,0))
                setPos(pos().x()+speed,pos().y());
        break;
        case Movement::UP:
            if(!allCollisions(walls, 0,-speed))
            setPos(pos().x(),pos().y()-speed);
        break;
        case Movement::LEFT:
            if(!allCollisions(walls, -speed,0))
            setPos(pos().x()-speed,pos().y());
        break;
        case Movement::DOWN:
            if(!allCollisions(walls, 0,speed))
            setPos(pos().x(),pos().y()+speed);
        break;
        default:
        break;
    }
}



void Character::setMovement(Movement mov)
{
    this->movement = mov;
}


Movement Character::getMovement()
{
    return this->movement;
}

bool Character::collision(bmb::Wall * wall, float speedX, float speedY)
{
    float top = getBounds().top();
    float bottom = getBounds().bottom();
    float left = getBounds().left();
    float right = getBounds().right();
    float wTop = wall->getBounds().top();
    float wBottom = wall->getBounds().bottom();
    float wLeft = wall->getBounds().left();
    float wRight = wall->getBounds().right();
    if(right + speedX >= wLeft && top + speedY <= wBottom && bottom + speedY >= wTop && left + speedX <= wRight)
        {
        return true;
        }
    return false;
}

bool Character::allCollisions(std::vector<bmb::Wall*> * walls, float speedX, float speedY)
{
    for(int i=0;i<walls->size();i++)
    {
        if(collision((*walls)[i],speedX,speedY))
            return true;
    }
    return false;
}

QRectF Character::getBounds()
{
    QRectF rect(pos().x()+24, pos().y()+52,18,8);
    return rect;
}


void Character::setBombs(int b)
{
    this->bombs = b;
}

int Character::getBombs()
{
    return this->bombs;
}


bool Character::bombCollision(float x, float y, float radius)
{
    float top = pos().y()+9;
    float bottom = pos().y()+60;
    float left = pos().x()+24;
    float right = pos().x()+42;
    float dis1 = sqrt((x-left)*(x-left) + (y-top)*(y-top));
    float dis2 = sqrt((x-left)*(x-left) + (y-bottom)*(y-bottom));
    float dis3 = sqrt((x-right)*(x-right) + (y-top)*(y-top));
    float dis4 = sqrt((x-right)*(x-right) + (y-bottom)*(y-bottom));
    if(dis1 <= radius || dis2 <= radius || dis3 <= radius || dis4 <= radius)
    {
        return true;
    }
    return false;
}

bool Character::getIsAlive()
{
    return isAlive;
}

void Character::setSpeed(float v)
{
    this->speed = v;
}
float Character::getSpeed()
{
    return this->speed;
}
