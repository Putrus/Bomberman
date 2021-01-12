#include "Wall.h"

using namespace bmb;

Wall::Wall(QString name, QPointF pos, QRectF bounds, bool isBreakable, Object * parent)  : Object(name, pos, bounds, parent)
{
    this->isBreakable = isBreakable;
}



void Wall::nextFrame()
{
    if(isBreakable && getAnimation() == Animation::BREAK)
    {
        if(getCurrentFrame().x() != getSpriteSize()*2)
        {
        setCurrentFrame(getCurrentFrame().x() + getSpriteSize(), getCurrentFrame().y() );
        }
        else{
            setToDeleted(true);
        }
    }
    update(0.f,0.f,getSpriteSize(),getSpriteSize());
}


