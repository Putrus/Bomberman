#include "Game.h"
#include "Bomberman.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Bomberman * bomberman = new Bomberman();
    bomberman->show();
    int r = a.exec();
    return r;
}
