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

    memset(buffer, 0, 255);
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
        //res == 1 oznacza, ze client sie rozlaczyl. Pozdro
        //tutaj akcje servera
        if(strcmp(action, "one") == 0)
        {
            break;
        }
        if(strcmp(action, "deleteClient") == 0)
        {
            Client * clientToRemove = &(*(Client*)ee.data.ptr);
            removeClient(clientToRemove);
        }
        if(action[0] == 'c')
        {
           char roomName[255];
           memset(roomName,0,255);
           strncat(roomName, action+2,strlen(action)-3);
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
        if(action[0] == 'j')
        {
            Client * client = &(*(Client*)ee.data.ptr);
            char roomName[255];
            memset(roomName,0,255);
            strncat(roomName, action+1,strlen(action)-5);
            std::cout << "Room name to join: "<<roomName<<std::endl;
            for(Room * room : rooms)
            {
                if(strcmp(room->getName(), roomName) == 0)
                {
                    if((int)room->getClients().size() < room->getMaxPlayers())
                    {
                        client->write(action, strlen(action) - 3);
                        room->addClient(client);
                        char * roomInfoChar = roomInfo();
                        sendToAll(roomInfoChar, strlen(roomInfoChar));
                    }
                }
            }
        }
        
        memset(action, 0, 255);
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
            strcpy(bufferInfo,"one");
            return bufferInfo;
        }

        std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << " assigned fd: " << clientFd << std::endl; 
        Client * client = new Client(clientFd, epollFd);
        clients.insert(client);
        char * m = roomInfo();
        client->write(buffer, strlen(m));
        
    }
    if(events & ~EPOLLIN){
        error(1, 0, "Event %x on server socket", events);
        strcpy(bufferInfo,"one");
        return bufferInfo;
    }
    strcpy(bufferInfo,"newConnection");
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
    memset(buffer,0,255);
    strcpy(buffer,"r ");
    for(Room * room : rooms)
    {
        strncat(buffer, room->getName(), strlen(room->getName()));
        char mp = room->getMaxPlayers() + '0';
        char pl = (int)room->getClients().size() + '0';
        char info[4] = {' ',pl,'/', mp};
        strncat(buffer, info, 4);
        strcat(buffer, ";\0");
    }
    return buffer;
}




