#include "Client.h"


Client::Client(int fd, int epollFd)
{
    this->fd = fd;
    this->epollFd = epollFd;
    this->roomNumber = -1;
    epoll_event ee {EPOLLIN|EPOLLRDHUP,{.ptr=this}};
    epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ee);
}


Client::~Client()
{
    epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr);
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

char Client::handleEvent(uint32_t events){
if(events & EPOLLIN)
{
    
    ssize_t count = read(fd, readMessage.dataPos(), readMessage.remaining());
    if(count <= 0)
    {
        events |= EPOLLERR;
    }
    else{
        return readMessage.getData()[0];
        memset(readMessage.getData(), 0, readMessage.getLen());
    }
    return '0';

}
if(events & EPOLLOUT) {
    do {
        int remaining = messagesToWrite.front().remaining();
        int sent = send(fd, messagesToWrite.front().getData()+messagesToWrite.front().getPos(), remaining, MSG_DONTWAIT);
        if(sent == remaining) {
            messagesToWrite.pop_front();
            if(0 == messagesToWrite.size()) {
                waitForWrite(false);
                break;
            }
            continue;
        } else if(sent == -1) {
            if(errno != EWOULDBLOCK && errno != EAGAIN)
                events |= EPOLLERR;
        } else
            messagesToWrite.front().setPos(messagesToWrite.front().getPos() + sent);
    } while(false);
}

if(events & ~(EPOLLIN|EPOLLOUT))
{   
    return 1;
}
    return 0;
}



char Client::write(char * buffer, int count){
    if(messagesToWrite.size() != 0)
    {
        messagesToWrite.emplace_back(buffer, count);
        return '0';
    }
    int sent = send(fd, buffer, count, MSG_DONTWAIT);
    if(sent == count)
    {
        return '0';
    }
    if(sent == -1)
    {
        if(errno != EWOULDBLOCK && errno != EAGAIN){
            //zwraca 1 czyli client do usuniecia
            return '2';
        }
        messagesToWrite.emplace_back(buffer, count);
    }else
    {
        messagesToWrite.emplace_back(buffer + sent, count - sent);    
    }
    waitForWrite(true);
    return '0';
}



void Client::waitForWrite(bool epollout){
    epoll_event ee {EPOLLIN|EPOLLRDHUP|(epollout?EPOLLOUT:0), {.ptr=this}};
    epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &ee);
}



Message Client::getReadMessage()
{
    return readMessage;
}