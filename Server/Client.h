#include <iostream>
#include <poll.h>
#include <string.h>
#include <list>
#include "Handler.h"
#include "Message.cpp"

class Client : public Handler
{
public:
    int fd;
    int epollFd;
    int roomNumber;
    Message readMessage;
    std::list<Message> messagesToWrite;
    void waitForWrite(bool epollout);
public:
    Client(int fd, int epollFd);
    virtual ~Client();
    virtual char handleEvent(uint32_t events) override;
    char write(char * buffer, int count);
    //getters and setters
    Message getReadMessage();
};