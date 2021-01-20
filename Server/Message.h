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
#include <vector>

class Message{
private:
    char * data;
    ssize_t len = 16;
    ssize_t pos = 0;
public:
    Message();
    Message(const char * data, ssize_t len);
    ~Message();
    void doubleCapacity();
    ssize_t remaining();
    char * dataPos();

    //getters and setters
    ssize_t getLen();
    ssize_t getPos();
    char * getData();
    void setLen(ssize_t len);
    void setPos(ssize_t pos);
    void setData(char *data);

};