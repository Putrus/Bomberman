#include "Bomberman.h"
#include "ui_bomberman.h"

Bomberman::Bomberman(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Bomberman)
{

    QFontDatabase::addApplicationFont("small_pixel.ttf");
    QFont pixelFont("Small Pixel", QFont::Normal);
    ui->setupUi(this);
    game = NULL;
    ui->centralwidget->setFont(pixelFont);
    setWindowTitle("Bomberman");
    hideAll();
    readUsername();
    ui->textEditPort->setText("5000");
    ui->textEditAddress->setText("localhost");
    buttonConnects();
    this->playerNumber = 0;
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
    ui->roomMenu->hide();
    if(game!=NULL)
    {
        game->hide();
        delete game;
        game = NULL;
    }
    ui->menu->show();

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
    connect(ui->connectButton, &QPushButton::clicked, this, &Bomberman::connectBtnHit);
    connect(ui->backOnlineButton, &QPushButton::clicked, this, &Bomberman::socketDisconnected);
    connect(ui->leaveRoomButton, &QPushButton::clicked, this, &Bomberman::leaveBtnHit);
    //optionsMenu
    connect(ui->optionsButton, &QPushButton::clicked, [=](){this->showMenu(ui->optionsMenu, ui->menu);});
    connect(ui->backOptionsButton, &QPushButton::clicked, [=](){this->showMenu(ui->menu, ui->optionsMenu);});
    connect(ui->changeUsernameButton, &QPushButton::clicked, [=](){this->setUsername(ui->textEditUsername->toPlainText());});
    //roomsMenu
    connect(ui->createButton, &QPushButton::clicked, this, &Bomberman::createRoom);
    connect(ui->joinButton, &QPushButton::clicked, this, &Bomberman::joinBtnHit);
    //roomMenu
    connect(ui->startGameButton, &QPushButton::clicked, this, &Bomberman::startBtnHit);

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


void Bomberman::connectBtnHit()
{
    ui->textEditConnectedInfo->append("<b>Connecting to " + ui->textEditAddress->toPlainText() + ":" + ui->textEditPort->toPlainText()+"</b>");

    sock = new QTcpSocket(this);

    connTimeoutTimer = new QTimer(this);
    connTimeoutTimer->setSingleShot(true);
    connect(connTimeoutTimer, &QTimer::timeout, [&]{
        connTimeoutTimer->deleteLater();
        ui->textEditConnectedInfo->append("<b>Connect timed out</b>");
        QMessageBox::critical(this, "Error", "Connect timed out");
    });
    connect(sock, &QTcpSocket::connected, this, &Bomberman::socketConnected);
    connect(sock, &QTcpSocket::disconnected, this, &Bomberman::socketDisconnected);
    connect(sock, &QTcpSocket::readyRead, this, &Bomberman::socketReadable);
    sock->connectToHost(ui->textEditAddress->toPlainText(), ui->textEditPort->toPlainText().toInt());
    connTimeoutTimer->start(3000);

}


void Bomberman::joinBtnHit()
{
    if(!ui->listRooms->selectedItems().isEmpty())
    {
    QString joinRoom = ui->listRooms->selectedItems().first()->text();
    joinRoom = joinRoom.mid(0,joinRoom.length()-4);
    ui->labelRoomName2->setText(joinRoom);
    qDebug() << joinRoom << "  joinroom ";
    sendMessage("j" + joinRoom + ";");
    }
}

void Bomberman::leaveBtnHit()
{
    sendMessage("l;");
    showMenu(ui->onlineMenu, ui->roomMenu);
}

void Bomberman::startBtnHit()
{
    qDebug() << ui->labelPlayers->text()[1];
    if(ui->labelPlayers->text()[1] == '2' || ui->labelPlayers->text()[1] == '3' || ui->labelPlayers->text()[1] == '4')
    {
        sendMessage("s;");
    }
}

void Bomberman::socketConnected()
{
    connTimeoutTimer->stop();
    connTimeoutTimer->deleteLater();
    ui->textEditConnectedInfo->append("<b>Connected!</b>");
    this->showMenu(ui->onlineMenu, ui->connectMenu);
}

void Bomberman::socketDisconnected()
{
    sock->disconnect();
    sock->close();
    QMessageBox::critical(this, "Error", "Disconnected frm the server!");
    ui->textEditConnectedInfo->append("<b>Disconnected from the server!</b>");
    this->hideAll();
}


void Bomberman::sendMessage(QString message)
{
    sock->write(message.toUtf8(), message.length());
}

void Bomberman::createRoom()
{
    QString maxPlayers = ui->textEditMaxPlayers->toPlainText();
    QString roomName = ui->textEditRoomName->toPlainText();
    if((maxPlayers == "2" || maxPlayers == "3" || maxPlayers == "4") && roomName.length() > 0)
    {
    sendMessage("c" + maxPlayers + roomName.toUpper() + ";");
    sendMessage("j" + roomName.toUpper() + ";");
    ui->labelRoomName2->setText(roomName.toUpper());
    }
}


void Bomberman::socketReadable()
{
    QByteArray b = sock->readAll();
    QString message = QString(b);
    if(message.length()>0)
    {
    std::vector<QString> actions;
    int counter = 0;
    for(int i=0;i<(int)message.length();i++)
    {
        if(message[i] == ';')
        {
            QString action = message.mid(counter, i-counter);
            counter = i+1;
            actions.push_back(action);
        }
    }
    for(auto it = actions.begin();it!=actions.end();++it)
    {
    if((*it)[0] == 'r')
    {
        ui->listRooms->clear();
        it->remove(0,1);
        int counter = 0;
        for(int i=0;i<it->length();i++)
        {
            if((*it)[i] == ',')
            {
                QString roomName = it->mid(counter, i-counter);
                counter = i+1;
                ui->listRooms->addItem(roomName);
                if(ui->labelRoomName2->text() == roomName.mid(0, roomName.length()-4))
                {
                    ui->labelPlayers->setText(roomName.mid(roomName.length()-4, 4));
                }
            }
        }
    }
    if((*it)[0] == 'j')
    {
        showMenu(ui->roomMenu, ui->onlineMenu);
    }

    if((*it)[0] == 's')
    {
        showMenu(ui->roomMenu, ui->onlineMenu);
        playerNumber = (*it)[1].digitValue();
        game = new Game((*it)[2].digitValue(), ui->centralwidget);
        game->show();
        ui->roomMenu->hide();
    }

    if((*it)[0] == 'g')
    {
        QChar action = (*it)[1];
        int k = (*it)[2].digitValue();
        std::vector<bmb::Character*> characters;
        bmb::Character * character = game->getCharacter(k);
        if(action == 'u')
        {
            character->setAction(bmb::Animation::UP);
        }
        if(action == 'l')
        {
            character->setAction(bmb::Animation::LEFT);
        }
        if(action == 'r')
        {
            character->setAction(bmb::Animation::RIGHT);
        }
        if(action == 'd')
        {
            character->setAction(bmb::Animation::DOWN);
        }
        if(action == 'y')
        {
            character->setAction(bmb::Animation::IDLE_UP);
        }
        if(action == 'k')
        {
            character->setAction(bmb::Animation::IDLE_LEFT);
        }
        if(action == 't')
        {
            character->setAction(bmb::Animation::IDLE_RIGHT);
        }
        if(action == 'c')
        {
            character->setAction(bmb::Animation::IDLE_DOWN);
        }
        if(action == 'b')
        {
            game->createBomb(character);
        }

    }




    }
    actions.clear();
    actions.shrink_to_fit();
}
}






//sterowanie
void Bomberman::keyPressEvent(QKeyEvent *event)
{
    if(game != NULL)
    {
    switch(event->key())
    {
    case Qt::Key_W:
        //(*characters)[0]->setAction(bmb::Animation::UP);
        sendMessage("gu"+QString::number(playerNumber)+";");
        break;
    case Qt::Key_A:
        //(*characters)[0]->setAction(bmb::Animation::LEFT);
        sendMessage("gl"+QString::number(playerNumber)+";");
        break;
    case Qt::Key_S:
        //(*characters)[0]->setAction(bmb::Animation::DOWN);
        sendMessage("gd"+QString::number(playerNumber)+";");
        break;
    case Qt::Key_D:
        //(*characters)[0]->setAction(bmb::Animation::RIGHT);
        sendMessage("gr"+QString::number(playerNumber)+";");
        break;
    }
    }
}

void Bomberman::keyReleaseEvent(QKeyEvent *event)
{
    if(game != NULL)
    {
    bmb::Character * character = game->getCharacter(playerNumber);
    switch(event->key())
    {
    case Qt::Key_W:
        if(character->getAnimation() == bmb::Animation::UP)
            sendMessage("gy"+QString::number(playerNumber)+";");
        break;
    case Qt::Key_A:
        if(character->getAnimation() == bmb::Animation::LEFT)
            sendMessage("gk"+QString::number(playerNumber)+";");
        break;
    case Qt::Key_S:
        if(character->getAnimation() == bmb::Animation::DOWN)
            sendMessage("gc"+QString::number(playerNumber)+";");
        break;
    case Qt::Key_D:
        if(character->getAnimation() == bmb::Animation::RIGHT)
            sendMessage("gt"+QString::number(playerNumber)+";");
        break;
    case Qt::Key_Space:
        if(character->getBombs()>0)
        {
        sendMessage("gb"+QString::number(playerNumber)+";");

        }
        break;
    }
    }
}











