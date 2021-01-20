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
        char action = (((Handler*)ee.data.ptr)->handleEvent(ee.events));
        //res == 1 oznacza, ze client sie rozlaczyl. Pozdro
        //tutaj akcje servera
        if(action == '1')
        {
            break;
        }
        else if(action == '2')
        {
            Client * clientToRemove = &(*(Client*)ee.data.ptr);
            std::cout << "Removing client with fd: " << clientToRemove->fd << std::endl;
            clients.erase(clientToRemove);
            delete clientToRemove;
        }
        else if(res == '0')
        {
            break;
        }

    }
}


char Server::handleEvent(uint32_t events)
{
    if(events & EPOLLIN){
        sockaddr_in clientAddr{};
        socklen_t clientAddrSize = sizeof(clientAddr);
        
        int clientFd = accept(serverFd, (sockaddr*) &clientAddr, &clientAddrSize);
        if(clientFd == -1)
        {
            error(1, 0, "Accept failed!");
        }

        std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << " assigned fd: " << clientFd << std::endl; 
        
        clients.insert(new Client(clientFd, epollFd));
    }
    if(events & ~EPOLLIN){
        error(1, 0, "Event %x on server socket", events);
        return '1';
    }

    return '0';
}



void Server::sendToAll(char * buffer, int count)
{
    for(auto it = clients.begin(); it!=clients.end();it++)
    {
        (*it)->write(buffer, count);
    }
}