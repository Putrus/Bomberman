#pragma once
class Handler{
public:
    char buffer[255];
public:
    virtual ~Handler(){}; 
    virtual char * handleEvent(uint32_t events) = 0;
};