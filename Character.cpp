#include "Character.h"

using namespace bmb;

Character::Character(QString name, QPointF pos, QRectF bounds, Object *parent) : Object(name, pos, bounds, parent)
{
    step = 0;
    bombs = 1;
    speed = QPointF(0.f, 0.f);
    velocity = 0.15f;
    score = 0;
}


void Character::nextFrame()
{
    switch(getAnimation())
    {
    case Animation::IDLE_UP:
        calcFrame(getSpriteSize()*2, true);
        break;
    case Animation::IDLE_LEFT:
        calcFrame(getSpriteSize()*3, true);
        break;
    case Animation::IDLE_DOWN:
        calcFrame(0, true);
        break;
    case Animation::IDLE_RIGHT:
        calcFrame(getSpriteSize(), true);
        break;
    case Animation::UP:
        calcFrame(getSpriteSize()*6, false);
        break;
    case Animation::LEFT:
        calcFrame(getSpriteSize()*7, false);
        break;
    case Animation::DOWN:
        calcFrame(getSpriteSize()*4, false);
        break;
    case Animation::RIGHT:
        calcFrame(getSpriteSize()*5, false);
        break;
    case Animation::BREAK:
        calcFrame(-1.f,true);
        break;
    default:
        setCurrentFrame(QPointF(0.0f,0.0f));
        break;
    }
    this->update(0, 0, getSpriteSize(), getSpriteSize());
}

void Character::calcFrame(float frameY, bool idle)
{
    //czy postac zyje
    if(getAnimation() != Animation::BREAK)
    {
    if(idle == true)
    {
        setCurrentFrame(getCurrentFrame().x(), frameY);
        if(getCurrentFrame().x() == getSpriteSize())
        {
            setCurrentFrame(0.f, getCurrentFrame().y());
        }
        else{
            setCurrentFrame(getSpriteSize(), getCurrentFrame().y());
        }
    }
    else
    {
        if(getCurrentFrame().y() == frameY - getSpriteSize()*4)
        {
            setCurrentFrame(getCurrentFrame().x(), frameY);
            if(step == 0)
            {
                setCurrentFrame(0.f, getCurrentFrame().y());
                step = 1;
            }else{
                setCurrentFrame(getSpriteSize(), getCurrentFrame().y());
                step = 0;
            }
        }
        else{
            setCurrentFrame(0.f, frameY - getSpriteSize()*4);
        }
    }
    }
    else{
        setCurrentFrame(getCurrentFrame().x(), getSpriteSize() * 8);
        if(getCurrentFrame().x() == getSpriteSize()*2)
        {
            setToDeleted(true);
            return;
        }
        setCurrentFrame(getCurrentFrame().x() + getSpriteSize(), getCurrentFrame().y());
    }
}


void Character::setAction(Animation action)
{
    setAnimation(action);
    switch(action)
    {
    case Animation::UP:
        setSpeed(0.f, -velocity);
        break;
    case Animation::LEFT:
        setSpeed(-velocity, 0.f);
        break;
    case Animation::DOWN:
        setSpeed(0.f, velocity);
        break;
    case Animation::RIGHT:
        setSpeed(velocity, 0.f);
        break;
    default:
        setSpeed(0.f, 0.f);
        break;
    }
}


void Character::gameUpdate(bool isDamage, bool canMove)
{
    qDebug() << isDamage << " " << canMove;
    if(isDamage)
    {
        setAction(Animation::BREAK);
        return;
    }
    if(canMove)
    {
        setPos(pos() + speed);
    }

}


int Character::getBombs()
{
    return bombs;
}

void Character::setBombs(int b)
{
    this->bombs = b;
}

QPointF Character::getSpeed()
{
    return speed;
}

void Character::setSpeed(QPointF speed)
{
    this->speed = speed;
}

void Character::setSpeed(float speedX, float speedY)
{
    this->speed = QPointF(speedX, speedY);
}


float Character::getVelocity()
{
    return velocity;
}

void Character::setVelocity(float velocity)
{
    this->velocity = velocity;
}

int Character::getScore()
{
    return score;
}

void Character::setScore(int score)
{
    this->score = score;
}

void Character::addScore(int add)
{
    this->score+=add;
}
