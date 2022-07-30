#include <iostream>
#include <time.h>
#include <chrono>
#include "Board.h"
#include "Piece.h"
#include "Engine.h"

int main()
{
    Board b;
    Engine TalSim(b);
    // b.Move("nc3");
    // b.Move("pe5");
    // b.Move("nd5");
    // b.Move("ke7");
    // b.Move("ne7");
    // b.PrintPosition(b.current);
    // b.DisplayMoves(b.current);

    auto start = std::chrono::steady_clock::now();

    Tree tree(b.current);
    Tree* t = &tree;
    TalSim.BuildTree(4, t);

    auto end = std::chrono::steady_clock::now();  
    auto duration = end - start;

    std::cout << std::endl << "time: " << std::chrono::duration <double> (duration).count() << std::endl;


    std::vector<Tree*> branches = t->GetBranches(2);
    int n_branch = branches.size();
    srand(time(NULL));
    b.PrintPosition(branches[10000]->position);
    return 0;
}