#include "Message.h"


Message::Message()
{
    data = (char*) malloc(len);
    //data = new char(len);
}

Message::Message(const char* data, ssize_t len)
{
    this->len = len;
    this->data = (char*) malloc(len);
    //data = new char(len);
    memcpy(this->data, data, len);
}

Message::~Message()
{
    free(data);
}

void Message::doubleCapacity()
{
    len<<=1;
    data = (char*) realloc(data, len);
}

ssize_t Message::remaining()
{
    return len - pos;
}

char * Message::dataPos()
{
    return data + pos;
}

ssize_t Message::getLen()
{
    return len;
}

ssize_t Message::getPos()
{
    return pos;
}

char * Message::getData()
{
    return data;
}

void Message::setLen(ssize_t len)
{
    this->len = len;
}

void Message::setPos(ssize_t pos)
{
    this->pos = pos;
}

void Message::setData(char * data)
{
    this->data = data;
}

