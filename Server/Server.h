#pragma once
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <sys/epoll.h> 
#include <unordered_set>
#include <signal.h>
#include <string.h>
#include <string>
#include "Handler.h"
#include "Room.cpp"

class Server : public Handler
{
private:
    int serverFd;
    //epoll
    int epollFd;
    //to jeszcze do ogarniecia
    std::unordered_set<Client*> clients;
    std::unordered_set<Room*> rooms;
    char buffer[255];
    char bufferInfo[255];
public:
    Server(int port);
    virtual ~Server();
    void start();
    char * handleEvent(uint32_t events) override;
    void sendToAll(char * buffer, int count);
    bool ifRoomNameExists(char * name);
    void removeClient(Client * client);
    char * roomInfo();
};