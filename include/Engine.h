#ifndef ENGINE
#define ENGINE

#include <iostream>
#include <unordered_map>
#include "Position.h"
#include "Board.h"
#include "Tree.h"
#include "Helpers.h"

class Engine
{
    public:
    void SuperEvaluate(Position*& pos);
    void BuildTree(const int depth, Tree*& t);
    Tree* tree;
    Engine(Board& b);
    ~Engine();

    private:
    float value[8];
    std::unordered_map<Position, Tree*, HashPolicy> transpose;
    Board& board;

    void Evaluate(Position*& pos);
    void Transpose();

};

#endif