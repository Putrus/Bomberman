#pragma once
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <error.h>
#include <string.h>
#include <list>
#include <vector>


class Client{
public:
    int fd;
    char message[255];
    Client(int fd);
    void readMessage();
    void sendMessage(char *mess);
};