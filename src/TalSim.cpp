#include <iostream>
#include <time.h>
#include "Board.h"
#include "Piece.h"
#include "Engine.h"

int main()
{
    Board b;
    // Engine TalSim(b);

    b.Move("pe4");
    b.Move("pd5");
    b.Move("qe2");
    b.Move("pe6");
    b.Move("pd5");
    b.PrintPosition(b.current);
    b.DisplayMoves(b.current);

    // Tree tree(b.current);
    // Tree* t = &tree;
    // TalSim.BuildTree(4, t);
    // std::vector<Tree*> branches = t->GetBranches(4);
    // int n_branch = branches.size();
    // srand(time(NULL));
    // b.PrintPosition(branches[rand() % n_branch]->position);
    return 0;
}