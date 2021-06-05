#ifndef CACHE_H // include guard
#define CACHE_H
#include <iostream>

class Cache{
public:
    int tag;
    bool valid;
    bool dirty;
    Cache(){
        dirty = false;
        valid = false;
    }
    private:
};

#endif