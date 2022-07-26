#ifndef TREE
#define TREE

#include<iostream>
#include<vector>
#include<utility>
#include "Position.h"

struct Tree
{
    int layer;
    Position position;
    std::vector<Tree*> branches;
    std::vector<bool> alive;

    std::vector<Tree*> GetBranches(const int depth);
    Tree(Position& pos, int l = 0);
    Tree();
    ~Tree();

    private:
    std::vector<Tree*>& GetBranches(const int depth, std::vector<Tree*>& branch);
};

#endif