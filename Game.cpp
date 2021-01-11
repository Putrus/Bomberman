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
    walls = new std::vector<Wall*>();
    for(int i=1;i<7;i++)
    {
        for(int j=1;j<7;j++)
        {
            Wall *wall = new Wall(i*68,j*68);
            walls->push_back(wall);
            scene->addItem(wall);
        }
    }
    bombs = new std::vector<Bomb*>();
    timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, character, [=](){character->move(walls);});
    QObject::connect(timer, &QTimer::timeout, this, &Game::checkBombCollisions);
    timer->start(17);

}


void Game::mousePressEvent(QMouseEvent *ev)
{
    Character character;
    Bomb *bomb = new Bomb(character, ev->x()-64, ev->y()-64);
    scene->addItem(bomb);
}


void Game::keyPressEvent(QKeyEvent *event)
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


void Game::keyReleaseEvent(QKeyEvent *event)
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
        Bomb *bomb = new Bomb(*character, character->pos().x()-32, character->pos().y()-20);
        bomb->setZValue(5);
        scene->addItem(bomb);
        bombs->push_back(bomb);
        }
        break;
    }
}



void Game::checkBombCollisions()
{
    for(auto it = bombs->begin();it != bombs->end(); ++it)
    {
        if((*it)->pos().x() == -5)
        {
            bombs->erase(it);
            delete *it;
            qDebug() << bombs->size();

            continue;
        }
        else{
        float x = (*it)->pos().x() + 63;
        float y = (*it)->pos().x() + 72;
        float top = character->pos().x()+12;
        float bottom = character->pos().y()+25;
        float left = character->pos().y()+5;
        float right = character->pos().x()+9;
        if(character->bombCollision(x,y,16))
        {

        }

        }
    }
}
