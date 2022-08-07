#include <iostream>
#include <chrono>
#include "Board.h"
#include "Piece.h"
#include "Engine.h"

int main()
{
    Board b;
    Engine TalSim(b);

    auto start = std::chrono::steady_clock::now();  

    b.Move("pe4");
    b.Move("pf5");
    b.Move("pe5");
    b.Move("pd5");
    b.Move("pd6");
    b.Move("pb6");
    b.Move("pd7");
    b.Move("kf7");
    b.Move("pc8", 'n');
    b.Move("qe8");
    b.Move("nd6");

    auto end = std::chrono::steady_clock::now();  
    auto duration = end - start;

    std::cout << std::endl << "time: " << std::chrono::duration <double> (duration).count() << std::endl;

    b.PrintPosition(b.current);
    b.DisplayMoves(b.current);

    // auto start = std::chrono::steady_clock::now();  

    // TalSim.SuperEvaluate(b.current, 2);

    // auto end = std::chrono::steady_clock::now();  
    // auto duration = end - start;

    // std::cout << std::endl << "time: " << std::chrono::duration <double> (duration).count() << std::endl;
    
    // std::vector<Tree*> branches = TalSim.tree.GetBranches(2);
    // std::cout << std::endl << "branches: " << branches.size() << std::endl;;
    // srand(time(NULL));
    // b.PrintPosition(branches[rand() % branches.size()]->position);

    return 0;
}