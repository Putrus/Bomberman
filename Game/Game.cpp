#include "Game.h"
Game::Game(int players, QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,512,512);
    setScene(scene);
    setFixedSize(512,512);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //test
    objects = new std::vector<bmb::Object*>();
    characters = new std::vector<bmb::Character*>();
    for(int i=0;i<players;i++)
    {
        QPointF startPlace(0.0f,0.0f);
        if(i==0)
            startPlace = QPointF(15.f, 6.f);
        if(i==1)
            startPlace = QPointF(460.f, 6.f);
        if(i==2)
            startPlace = QPointF(15.f, 410.f);
        if(i==3)
            startPlace = QPointF(460.f, 410.f);
        bmb::Character * character = new bmb::Character("penguin"+QString::number(i),startPlace,QRectF(24.f,52.f,18.f,8.f));
        scene->addItem(character);
        characters->push_back(character);
        objects->push_back(character);
        character->setZValue(6);
    }
    //wallsy
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            bmb::Wall *wall = new bmb::Wall("wall",QPointF(i*68-12,j*68-12), QRectF(15.f,15.f,34.f,34.f), false);
            objects->push_back(wall);
            bmb::Wall *wall2 = new bmb::Wall("wall",QPointF(i*68+22,j*68-12), QRectF(15.f,15.f,34.f,34.f), true);
            objects->push_back(wall2);
            scene->addItem(wall);
            scene->addItem(wall2);
        }
    }
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<8;j++)
        {
            bmb::Wall *wall2 = new bmb::Wall("wall",QPointF(i*68+22+34,j*68-12+34), QRectF(15.f,15.f,34.f,34.f), true);
            objects->push_back(wall2);
            scene->addItem(wall2);
        }
    }
    for(int i=0;i<7;i++)
    {
        for(int j=1;j<6;j++)
        {
            bmb::Wall *wall2 = new bmb::Wall("wall",QPointF(i*68+22,j*68-12+34), QRectF(15.f,15.f,34.f,34.f), true);
            objects->push_back(wall2);
            scene->addItem(wall2);
        }
    }
    timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, this, &Game::updateGame);
    timer->start(1);
    animationTimer = new QTimer();
    QObject::connect(animationTimer, &QTimer::timeout, this, &Game::updateAnimation);
    animationTimer->start(100);
}






void Game::updateGame()
{
    for (auto it = objects->begin(); it != objects->end(); it++) {
        if((*it)[0].getToDeleted() == true)
        {
            delete (*it);
            *it = nullptr;
            objects->erase(it--);
            continue;
        }
        bmb::Bomb * b = dynamic_cast<bmb::Bomb*>(*it);
        if(b == NULL)
        {
            b = NULL;
            delete b;
            bmb::Character * c = dynamic_cast<bmb::Character*>(*it);
            if(c == NULL)
            {
                (*it)->gameUpdate(allBombsCollision((*it), objects));
            }else{
                (*it)->gameUpdate(allBombsCollision((*it), objects), !allCollisions(c, objects));
            }
            c = NULL;
            delete c;
        }
    }

}


void Game::updateAnimation()
{
    for (auto it = objects->begin(); it != objects->end(); it++) {
        (*it)->nextFrame();
    }
}

bool Game::bombCollision(bmb::Object *object, bmb::Bomb * bomb)
{
    if(bomb->ifExplodes())
    {
    float pow1 = (bomb->pos().x()+63 - object->getBounds().x()) * (bomb->pos().x()+63 - object->getBounds().x());
    float pow2 = (bomb->pos().x()+63 - object->getBounds().right()) * (bomb->pos().x()+63 - object->getBounds().right());
    float pow3 = (bomb->pos().y()+72 - object->getBounds().y()) * (bomb->pos().y()+72 - object->getBounds().y());
    float pow4 = (bomb->pos().y()+72 - object->getBounds().bottom()) * (bomb->pos().y()+72 - object->getBounds().bottom());
    float dis1 = sqrt(pow1 + pow3);
    float dis2 = sqrt(pow1 + pow4);
    float dis3 = sqrt(pow2 + pow3);
    float dis4 = sqrt(pow2 + pow4);
    float radius = bomb->getExplosionRadius();
    if(dis1 <= radius || dis2 <= radius || dis3 <= radius || dis4 <= radius)
    {
        return true;
    } 
    }
    return false;
}

bool Game::collision(bmb::Character *character, bmb::Object *object)
{
    bmb::Bomb * b = dynamic_cast<bmb::Bomb*>(object);
    if(b == NULL || &(*b->getCharacter()) != &(*character))
    {
        b = NULL;
        delete b;
    float top1 = character->getBounds().top();
    float bottom1 = character->getBounds().bottom();
    float left1 = character->getBounds().left();
    float right1 = character->getBounds().right();
    float top2 = object->getBounds().top();
    float bottom2 = object->getBounds().bottom();
    float left2 = object->getBounds().left();
    float right2 = object->getBounds().right();
    if(right1 + character->getSpeed().x() > left2 && top1 + character->getSpeed().y() < bottom2 && bottom1 + character->getSpeed().y() > top2 && left1 + character->getSpeed().x() < right2)
    {
        return true;
    }
    if(right1 + character->getSpeed().x() > 510 || left1 + character->getSpeed().x() < 0 || bottom1 + character->getSpeed().y() > 510 || top1 + character->getSpeed().y() < 10)
    {
        return true;
    }
    }

    b = NULL;
    delete b;
    return false;
}

bool Game::allBombsCollision(bmb::Object * object, std::vector<bmb::Object*> * objects)
{
    for (auto it = objects->begin(); it != objects->end(); it++) {
        bmb::Bomb * b = dynamic_cast<bmb::Bomb*>(*it);
        if(b != NULL)
        {
            if(bombCollision(object, b))
            {
                b = NULL;
                delete b;
                return true;
            }
        }
        b = NULL;
        delete b;
    }
    return false;
}

bool Game::allCollisions(bmb::Character *character, std::vector<bmb::Object*> * objects)
{
    for (auto it = objects->begin(); it != objects->end(); it++) {
        bmb::Character * c = dynamic_cast<bmb::Character*>(*it);
        if(c == NULL)
        {
            delete c;
            if(collision(character, (*it)))
            {
                return true;
            }
        }
    }
    return false;
}


bmb::Character * Game::getCharacter(int number)
{
    return (*this->characters)[number];
}


std::vector<bmb::Object*> * Game::getObjects()
{
    return objects;
}

QGraphicsScene * Game::getScene()
{
    return scene;
}

void Game::createBomb(bmb::Character *character)
{
    bmb::Bomb *bomb = new bmb::Bomb((*character), "bomb_sprite", QPointF(-420.69f, -420.69f), QRectF(52.f, 58.f, 22.f, 24.f));
    bomb->setZValue(5);
    scene->addItem(bomb);
    objects->push_back(bomb);
}
