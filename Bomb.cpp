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
        return;
    }
    if(getCurrentFrame().x() == 1152)
    {
        setCurrentFrame(0.0f,getCurrentFrame().y() + 128);
    }
    this->update(0, 0, getSpriteSize(), getSpriteSize());
}

bool Bomb::ifExplodes()
{
    if(getCurrentFrame().y() == 128)
    {
        return true;
    }
    return false;
}
