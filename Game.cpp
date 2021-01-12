#include "Game.h"

Game::Game(QWidget *parent)
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,800,600);


    setScene(scene);
    setFixedSize(800,600);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    character = new Character(Name::PENGUIN_WELDER);
    character->setZValue(6);
    scene->addItem(character);
    walls = new std::vector<bmb::Wall*>();
    for(int i=1;i<7;i++)
    {
        for(int j=1;j<7;j++)
        {
            bmb::Wall *wall = new bmb::Wall("wall",QPointF(i*100,j*100), QRectF(15.f,15.f,34.f,34.f));
            walls->push_back(wall);
            scene->addItem(wall);
        }
    }
    bombs = new std::vector<bmb::Bomb*>();
    timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, character, [=](){character->move(walls);});
    QObject::connect(timer, &QTimer::timeout, this, &Game::checkBombCollisions);
    timer->start(17);


    bmb::Wall * object = new bmb::Wall("wall", QPointF(150.f,150.f), QRectF(15.f,15.f,34.f,34.f), true);
    scene->addItem(object);
    QTimer *t = new QTimer();
    QObject::connect(t, &QTimer::timeout, object, [=](){object->setAnimation(bmb::Animation::BREAK);});
    t->start(4000);
    QTimer *te = new QTimer();
    QObject::connect(te, &QTimer::timeout, object, &bmb::Wall::nextFrame);
    te->start(100);
    QTimer *tb = new QTimer();
    QObject::connect(tb, &QTimer::timeout, this, &Game::updateAnimation);
    tb->start(100);
}


void Game::mousePressEvent(QMouseEvent *ev)
{
    Character character;
    QString bomb_name = "bomb_sprite";
    bmb::Bomb *bomb = new bmb::Bomb(character, bomb_name, ev->pos());
    scene->addItem(bomb);
}


void Game::keyPressEvent(QKeyEvent *event)
{
    if(character->getIsAlive())
    {
    switch(event->key())
    {
    case Qt::Key_W:
        character->setMovement(Movement::UP);
        break;
    case Qt::Key_A:
        character->setMovement(Movement::LEFT);
        break;
    case Qt::Key_S:
        character->setMovement(Movement::DOWN);
        break;
    case Qt::Key_D:
        character->setMovement(Movement::RIGHT);
        break;
    }
    }

}


void Game::keyReleaseEvent(QKeyEvent *event)
{
    if(character->getIsAlive())
    {
    switch(event->key())
    {
    case Qt::Key_W:
        if(character->getMovement() == Movement::UP)
        character->setMovement(Movement::IDLE_UP);
        break;
    case Qt::Key_A:
        if(character->getMovement() == Movement::LEFT)
        character->setMovement(Movement::IDLE_LEFT);
        break;
        break;
    case Qt::Key_S:
        if(character->getMovement() == Movement::DOWN)
        character->setMovement(Movement::IDLE_DOWN);
        break;
        break;
    case Qt::Key_D:
        if(character->getMovement() == Movement::RIGHT)
        character->setMovement(Movement::IDLE_RIGHT);
        break;
        break;
    case Qt::Key_Space:
        if(character->getBombs()>0)
        {
        bmb::Bomb *bomb = new bmb::Bomb(*character, "bomb_sprite");
        bomb->setZValue(5);
        scene->addItem(bomb);
        bombs->push_back(bomb);
        }
        break;
    }
    }
}



void Game::checkBombCollisions()
{
    for (auto it = bombs->begin(); it != bombs->end(); it++) {
        if((*it)[0].getToDeleted() == true)
        {
            delete (*it);
            *it = nullptr;
            bombs->erase(it--);
        }
        else{
            if((*it)->ifExplodes() && character->bombCollision((*it)->pos().x()+63, (*it)->pos().y()+72, 26))
            {
                character->setMovement(Movement::DEAD);
            }
        }
    }
}


void Game::updateAnimation()
{
    for (auto it = bombs->begin(); it != bombs->end(); it++) {
        (*it)->nextFrame();
    }
}
