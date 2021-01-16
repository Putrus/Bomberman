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

#include "Client.h"



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
    int sockfd = socket(AF_INET, SOCK_STREAM,0);
    if(sockfd == -1)
    {
        perror("Socket failed!");
        return 1;
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons((int)port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int bindfd = bind(sockfd, (sockaddr*) &addr, sizeof(addr));
    if(bindfd == -1)
    {
        perror("Socket failed!");
        close(sockfd);
        return 1;
    }
    int listenfd = listen(sockfd, 1);
    if(listenfd == -1)
    {
        perror("Listen failed");
        close(sockfd);
        return 1;
    }
    std::vector<Client*> clients;
    while(1)
    {
        int fd = accept(sockfd, nullptr, nullptr);
        if(fd != -1)
        {
            Client * client = new Client(fd);
            clients.push_back(client);
            std::cout<<"New connection!"<<std::endl;
        }

    }


    close(sockfd);
}