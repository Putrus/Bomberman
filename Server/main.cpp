#include "Server.cpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <poll.h> 
#include <unordered_set>
#include <signal.h>



int main(int argc, char ** argv)
{
    if(argc!=2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }
    char * end;
    int port = strtol(argv[1], &end,10);
    if(*end != 0 || port < 1 || port > 65535){
        printf("Invalid argument: %s is not a port number\nUsage: %s <ip> <port>\n", argv[1], argv[0]);
        return 1;
    }
    Server *server = new Server(port);
    server->start();
    delete server;
    return 0;
}
