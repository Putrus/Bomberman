#include "Bomberman.h"
#include "ui_bomberman.h"

Bomberman::Bomberman(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Bomberman)
{
    QFontDatabase::addApplicationFont("small_pixel.ttf");
    QFont pixelFont("Small Pixel", QFont::Normal);
    ui->setupUi(this);
    ui->centralwidget->setFont(pixelFont);
    setWindowTitle("Bomberman");
    hideAll();
    ui->menu->show();
    readUsername();

    buttonConnects();



}

Bomberman::~Bomberman()
{
    delete ui;
}

void Bomberman::showMenu(QWidget * widgetShow, QWidget * widgetHide)
{
    if(widgetShow == nullptr && widgetHide == nullptr)
    {
        this->close();
        return;
    }
    widgetShow->show();
    widgetHide->hide();
}

void Bomberman::hideAll()
{
    ui->playMenu->hide();
    ui->optionsMenu->hide();
    ui->onlineMenu->hide();
    ui->connectMenu->hide();
}

void Bomberman::buttonConnects()
{
    //exit menu
    connect(ui->exitButton, &QPushButton::clicked, [=](){this->showMenu();});
    //playMenu
    connect(ui->playButton, &QPushButton::clicked, [=](){this->showMenu(ui->playMenu, ui->menu);});
    connect(ui->backPlayButton, &QPushButton::clicked, [=](){this->showMenu(ui->menu, ui->playMenu);});
    //connectMenu
    connect(ui->onlineButton, &QPushButton::clicked, [=](){this->showMenu(ui->connectMenu, ui->playMenu);});
    connect(ui->backConnectButton, &QPushButton::clicked, [=](){this->showMenu(ui->playMenu, ui->connectMenu);});
    //onlineMenu
    connect(ui->connectButton, &QPushButton::clicked, [=](){this->showMenu(ui->onlineMenu, ui->connectMenu);});
    connect(ui->backOnlineButton, &QPushButton::clicked, [=](){this->showMenu(ui->connectMenu, ui->onlineMenu);});
    //optionsMenu
    connect(ui->optionsButton, &QPushButton::clicked, [=](){this->showMenu(ui->optionsMenu, ui->menu);});
    connect(ui->backOptionsButton, &QPushButton::clicked, [=](){this->showMenu(ui->menu, ui->optionsMenu);});
    connect(ui->changeUsernameButton, &QPushButton::clicked, [=](){this->setUsername(ui->textEditUsername->toPlainText());});
}


void Bomberman::setUsername(QString username)
{
    if(username.length() > 0 && username.length() < 13)
    {
    this->username = username.toUpper();
    QFile file("data/info.txt");
    if(!file.open(QIODevice::Truncate|QIODevice::WriteOnly))
    {
            qDebug() << "Error opening file: " << file.error();
            //this->close();
            return;
    }
    file.write(this->username.toUtf8(), this->username.toUtf8().size());
    ui->username->setText(username);
    file.close();
    }
}

void Bomberman::readUsername()
{
    QFile file("data/info.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
            qDebug() << "Error opening file: " << file.error();
            this->close();
            return;
    }
    QTextStream instream(&file);
    QString username = instream.readLine();
    this->username = username;
    ui->username->setText(username);
    ui->textEditUsername->setText(username);
    file.close();
}
