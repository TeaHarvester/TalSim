#include <iostream>
#include "Board.h"
#include "Piece.h"
#include "Engine.h"

int main()
{
    Board b;
    Engine TalSim(b);
    Tree tree(b.current);
    Tree* t = &tree;
    TalSim.BuildTree(2, t);
    std::vector<Tree*> branches = t->GetBranches(1);
    std::cout << branches.size();
    b.PrintPosition(branches[300]->position);
    return 0;
}