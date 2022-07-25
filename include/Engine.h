#ifndef ENGINE
#define ENGINE

#include<iostream>
#include "Position.h"

class Engine
{
    public:
    void Evaluate(Position*& pos);
    Engine();

    private:
    float value[8];
};

#endif