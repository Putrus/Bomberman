#include "Bomb.h"

using namespace bmb;

Bomb::Bomb(Character & character, QString name, QPointF pos, QRectF bounds, Object *parent) : Object(name, pos, bounds, parent)
{
    this->character = &character;
    this->character->setBombs(this->character->getBombs()-1);
    explosionRadius = 26;
    if(pos == QPointF(-420.69f, -420.69f))
    {
        setPos(character.pos().x() - 32, character.pos().y() - 20);
    }
}




void Bomb::nextFrame()
{
    setCurrentFrame(getCurrentFrame().x()+128, getCurrentFrame().y());
    if(getCurrentFrame() == QPointF(1152.f, 128.f))
    {
        character->setBombs(character->getBombs()+1);
        setToDeleted(true);
    }
    if(getCurrentFrame().x() == 1152)
    {
        setCurrentFrame(0.0f,getCurrentFrame().y() + 128);
    }

    this->update(0, 0, getSpriteSize(), getSpriteSize());
}

void Bomb::gameUpdate(bool isDamage, bool canMove)
{
    if(!isDamage && canMove)
    {}
}

bool Bomb::ifExplodes()
{
    if(getCurrentFrame().y() == getSpriteSize())
    {
        return true;
    }
    return false;
}

float Bomb::getExplosionRadius()
{
    return explosionRadius;
}

void Bomb::setExplosionRadius(float radius)
{
    explosionRadius = radius;
}

Character * Bomb::getCharacter()
{
    return character;
}

void Bomb::setCharacter(Character &character)
{
    this->character = &character;
}
