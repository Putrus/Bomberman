#include "Client.h"



Client::Client(int fd)
{
    this->fd = fd;
    memset(message,0,255);
    fcntl(this->fd, F_SETFL, O_NONBLOCK, 1);
}

void Client::readMessage()
{
    memset(message,0,255);
    read(this->fd, this->message, 255);
}

void Client::sendMessage(char *mess)
{   
    write(this->fd, mess, 255);
}

