#include <iostream>
#include <chrono>
#include "Board.h"
#include "Piece.h"
#include "Engine.h"

int main()
{
    Board b;
    Engine TalSim(b);

    Tree tree(b.current);
    // Tree* t = &tree;

    auto start = std::chrono::steady_clock::now();  

    TalSim.BuildTree(5, tree);

    auto end = std::chrono::steady_clock::now();  
    auto duration = end - start;

    std::cout << std::endl << "time: " << std::chrono::duration <double> (duration).count() << std::endl;
    
    std::vector<Tree*> branches = tree.GetBranches(4);
    std::cout << std::endl << "branches: " << branches.size() << std::endl;;
    srand(time(NULL));
    b.PrintPosition(branches[10000]->position);
    return 0;
}