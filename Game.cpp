#include "Game.h"

Game::Game(QWidget *parent)
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,800,600);
    setScene(scene);
    setFixedSize(800,600);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //test
    objects = new std::vector<bmb::Object*>();

    characters = new std::vector<bmb::Character*>();
    bmb::Character * character = new bmb::Character("penguin_welder",QPointF(10.f,10.f), QRectF(24.f,52.f,18.f,8.f));
    character->setZValue(6);
    bmb::Character * character2 = new bmb::Character("penguin_welder",QPointF(500.f,500.f), QRectF(24.f,52.f,18.f,8.f));
    character2->setZValue(6);
    scene->addItem(character);
    scene->addItem(character2);
    characters->push_back(character);
    characters->push_back(character2);
    objects->push_back(character);
    objects->push_back(character2);
    for(int i=1;i<7;i++)
    {
        for(int j=1;j<7;j++)
        {
            bmb::Wall *wall = new bmb::Wall("wall",QPointF(i*100,j*100), QRectF(15.f,15.f,34.f,34.f), true);
            objects->push_back(wall);
            scene->addItem(wall);
        }
    }
    timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, this, &Game::updateGame);
    timer->start(1);
    animationTimer = new QTimer();
    QObject::connect(animationTimer, &QTimer::timeout, this, &Game::updateAnimation);
    animationTimer->start(100);
}


void Game::mousePressEvent(QMouseEvent *ev)
{
    ev->pos();
}


void Game::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_W:
        (*characters)[0]->setAction(bmb::Animation::UP);
        break;
    case Qt::Key_A:
        (*characters)[0]->setAction(bmb::Animation::LEFT);
        break;
    case Qt::Key_S:
        (*characters)[0]->setAction(bmb::Animation::DOWN);
        break;
    case Qt::Key_D:
        (*characters)[0]->setAction(bmb::Animation::RIGHT);
        break;
    case Qt::Key_Y:
        (*characters)[1]->setAction(bmb::Animation::UP);
        break;
    case Qt::Key_G:
        (*characters)[1]->setAction(bmb::Animation::LEFT);
        break;
    case Qt::Key_H:
        (*characters)[1]->setAction(bmb::Animation::DOWN);
        break;
    case Qt::Key_J:
        (*characters)[1]->setAction(bmb::Animation::RIGHT);
        break;
    }
}


void Game::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_W:
        if((*characters)[0]->getAnimation() == bmb::Animation::UP)
        (*characters)[0]->setAction(bmb::Animation::IDLE_UP);
        break;
    case Qt::Key_A:
        if((*characters)[0]->getAnimation() == bmb::Animation::LEFT)
        (*characters)[0]->setAction(bmb::Animation::IDLE_LEFT);
        break;
    case Qt::Key_S:
        if((*characters)[0]->getAnimation() == bmb::Animation::DOWN)
        (*characters)[0]->setAction(bmb::Animation::IDLE_DOWN);
        break;
    case Qt::Key_D:
        if((*characters)[0]->getAnimation() == bmb::Animation::RIGHT)
        (*characters)[0]->setAction(bmb::Animation::IDLE_RIGHT);
        break;
    case Qt::Key_Space:
        if((*characters)[0]->getBombs()>0)
        {
        bmb::Bomb *bomb = new bmb::Bomb(*(*characters)[0], "bomb_sprite", QPointF(-420.69f, -420.69f), QRectF(52.f, 58.f, 22.f, 24.f));
        bomb->setZValue(5);
        scene->addItem(bomb);
        objects->push_back(bomb);
        }
        break;
    case Qt::Key_Y:
        if((*characters)[1]->getAnimation() == bmb::Animation::UP)
        (*characters)[1]->setAction(bmb::Animation::IDLE_UP);
        break;
    case Qt::Key_G:
        if((*characters)[1]->getAnimation() == bmb::Animation::LEFT)
        (*characters)[1]->setAction(bmb::Animation::IDLE_LEFT);
        break;
    case Qt::Key_H:
        if((*characters)[1]->getAnimation() == bmb::Animation::DOWN)
        (*characters)[1]->setAction(bmb::Animation::IDLE_DOWN);
        break;
    case Qt::Key_J:
        if((*characters)[1]->getAnimation() == bmb::Animation::RIGHT)
        (*characters)[1]->setAction(bmb::Animation::IDLE_RIGHT);
        break;
    case Qt::Key_K:
        if((*characters)[1]->getBombs()>0)
        {
        bmb::Bomb *bomb = new bmb::Bomb(*(*characters)[1], "bomb_sprite", QPointF(-420.69f, -420.69f), QRectF(52.f, 58.f, 22.f, 24.f));
        bomb->setZValue(5);
        scene->addItem(bomb);
        objects->push_back(bomb);
        }
        break;
    }
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
    return false;
    }
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
