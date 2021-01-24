#pragma once
class Handler{
public:
    virtual ~Handler(){}; 
    virtual char * handleEvent(uint32_t events) = 0;
};