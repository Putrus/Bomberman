#include "Server.h"


Server::Server(int port)
{
    serverFd = socket(AF_INET, SOCK_STREAM,0);
    if(serverFd == -1)
    {
        error(1, 0, "Socket failed!");
    }
    
    signal(SIGPIPE, SIG_IGN);

    const int one = 1;
    int res = setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if(res == -1)
    {
        error(1, 0, "Setsockopt failed!");
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons((int)port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    res = bind(serverFd, (sockaddr*) &addr, sizeof(addr));
    if(res == -1)
    {
        error(1, 0, "Bind failed!");
    }

    res = listen(serverFd, 1);
    if(res == -1)
    {
        error(1, 0, "Listen failed!");
    }

    epollFd = epoll_create1(0);

    memset(bufferInfo, 0, 255);
}

Server::~Server()
{
    for(Client *client : clients)
        delete client;
    close(serverFd);
    std::cout << "Closing server!\n";
    exit(0);
}

void Server::start()
{
    epoll_event ee {EPOLLIN, {.ptr=this}};
    epoll_ctl(epollFd, EPOLL_CTL_ADD, serverFd, &ee);
    while(1)
    {
        int res = epoll_wait(epollFd, &ee, 1, -1);
        if(res == -1)
        {   
            error(1, 0, "Epoll_wait failed!");
            break;
        }
        char * action = (((Handler*)ee.data.ptr)->handleEvent(ee.events));
        Handler * handler = (Handler*)ee.data.ptr;
        //res == 1 oznacza, ze client sie rozlaczyl. Pozdro
        //tutaj akcje servera
        char *temp;
        char actions[255][255];
        int restRead = 0;
        int nActions = 0;
        for(int i=0;i<(int)strlen(action);i++)
        {
            if(action[i] == ';')
            {
                restRead = i;
            }
        }
        strncat(handler->buffer, action, restRead);
        temp = strtok(handler->buffer, ";");
        while(temp != NULL)
        {
            strcpy(actions[nActions++], temp);
            temp = strtok(NULL, ";");
        }
        
        for(int i=0;i<nActions;i++)
        {
        if(strcmp(actions[i], "one") == 0)
        {
            break;
        }
        if(strcmp(actions[i], "deleteClient") == 0)
        {
            Client * clientToRemove = &(*(Client*)ee.data.ptr);
            for(Room * room : rooms)
            {
                room->deleteClient(clientToRemove);
            }
            removeClient(clientToRemove);
            char * roomInfoChar = roomInfo();
            sendToAll(roomInfoChar, strlen(roomInfoChar));
        }
        if(actions[i][0] == 'c')
        {
           char roomName[255];
           memset(roomName,0,255);
           strncat(roomName, actions[i]+2,strlen(action)-2);
           if(!ifRoomNameExists(roomName))
           {
                Room * room = new Room(action[1]%48, roomName);
                rooms.insert(room);
                std::cout << "New room with name " << roomName << " created!" << std::endl; 
                char * roomInfoChar = roomInfo();
                sendToAll(roomInfoChar, strlen(roomInfoChar));
                
           }

           memset(roomName,0,255);
        }
        if(actions[i][0] == 'j')
        {
            Client * client = &(*(Client*)ee.data.ptr);
            char roomName[255];
            memset(roomName,0,255);
            strncat(roomName, actions[i]+1,strlen(actions[i]));
            for(Room * room : rooms)
            {
                if(strcmp(room->getName(), roomName) == 0)
                {
                    std::cout << "Client with fd: "<<client->fd <<  " join room " << room->getName()<<" succesfully!"<<std::endl;
                    if((int)room->getClients().size() < room->getMaxPlayers())
                    {
                        char mess[255];
                        strncat(mess, actions[i], strlen(actions[i]));
                        strcat(mess,";");
                        client->write(mess, strlen(mess));
                        room->addClient(client);
                        char * roomInfoChar = roomInfo();

                        sendToAll(roomInfoChar, strlen(roomInfoChar));
                        memset(mess,0,255);
                    }
                }
            }
        }
        if(actions[i][0] == 'l')
        {
            Client * client = &(*(Client*)ee.data.ptr);
            for(Room * room : rooms)
            {
                room->deleteClient(client);
                std::cout<<"Deleted client with fd: " <<client->fd<< " from room " << room->getName()<<std::endl;
            }
            char * roomInfoChar = roomInfo();
            sendToAll(roomInfoChar, strlen(roomInfoChar));
        }
        if(actions[i][0] == 's')
        {
            Client * client = &(*(Client*)ee.data.ptr);
            for(Room * room : rooms)
            {
                if(room->clientInRoom(client))
                {
                    char playerNumber = '0';
                    char playersNumber = (int)room->getClients().size()+'0';
                    for(Client * c : room->getClients())
                    {
                        char mess[255];
                        memset(mess,0,255);
                        strcat(mess, "s");
                        char tmp[2] = {playerNumber++, playersNumber};
                        strncat(mess, tmp, 2);
                        strcat(mess, ";");
                        std::cout << "Startujemy! "<<mess<<std::endl;
                        c->write(mess, strlen(mess));
                    }
                }
            }
            char * roomInfoChar = roomInfo();
            sendToAll(roomInfoChar, strlen(roomInfoChar));
        }
        if(actions[i][0] == 'g')
        {
            Client * client = &(*(Client*)ee.data.ptr);
            for(Room * room : rooms)
            {
                    if(room->clientInRoom(client))
                    {
                    for(Client * c : clients)
                    {
                        if(room->clientInRoom(c))
                        {
                        char mess[255];
                        strcpy(mess, actions[i]);
                        strcat(mess, ";");
                        c->write(mess, strlen(mess));
                        }
                    }
                    }
            }
            char * roomInfoChar = roomInfo();
            sendToAll(roomInfoChar, strlen(roomInfoChar));
        }
        memset(actions[i],0,255);
        }
        memset(handler->buffer,0,255);
        strncat(handler->buffer, action + restRead+1, strlen(action) - restRead);
        memset(action, 0, strlen(action));
    }
}


char * Server::handleEvent(uint32_t events)
{
    memset(bufferInfo,0,255);
    if(events & EPOLLIN){
        sockaddr_in clientAddr{};
        socklen_t clientAddrSize = sizeof(clientAddr);
        
        int clientFd = accept(serverFd, (sockaddr*) &clientAddr, &clientAddrSize);
        if(clientFd == -1)
        {
            error(1, 0, "Accept failed!");
            strcpy(bufferInfo,"one;");
            return bufferInfo;
        }

        std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << " assigned fd: " << clientFd << std::endl; 
        Client * client = new Client(clientFd, epollFd);
        clients.insert(client);
        char * m = roomInfo();
        client->write(m, strlen(m));
        
    }
    if(events & ~EPOLLIN){
        error(1, 0, "Event %x on server socket", events);
        strcpy(bufferInfo,"one;");
        return bufferInfo;
    }
    strcpy(bufferInfo,"newConnection;");
    return bufferInfo;
}



void Server::sendToAll(char * buffer, int count)
{
    for(Client * client : clients)
    {
        if(client->write(buffer, count) == -1)
        {
            removeClient(client);
        }
    }
}


bool Server::ifRoomNameExists(char * name)
{
    for(Room * room : rooms)
    {
        if(strcmp(room->getName(), name) == 0)
        {
            return true;
        }
    }
    return false;
}


void Server::removeClient(Client * client)
{
    std::cout << "Removing client with fd: " << client->fd << std::endl;
    clients.erase(client);
    delete client;
}

char * Server::roomInfo()
{
    memset(bufferInfo,0,255);
    strcpy(bufferInfo,"r");
    for(Room * room : rooms)
    {
        strncat(bufferInfo, room->getName(), strlen(room->getName()));
        char mp = room->getMaxPlayers() + '0';
        char pl = (int)room->getClients().size() + '0';
        char info[4] = {' ',pl,'/', mp};
        strncat(bufferInfo, info, 4);
        strcat(bufferInfo, ",\0");
    }
    strcat(bufferInfo,";\0");
    return bufferInfo;
}




