#include <chrono>
#include "Engine.h"

void Engine::SuperEvaluate(Position& pos, const int depth)
{
    tree = Tree(pos);
    BuildTree(tree);

    for (int layer = 0; layer < depth; ++layer)
    {
        std::vector<Tree*> branches = tree.GetBranches(layer);

        for (auto branchlet = branches.begin(); branchlet < branches.end(); ++branchlet)
        {
            BuildTree(**branchlet);
        }
    }
}

// void Engine::Evaluate(Position*& pos)
// {
//     float evaluation = 0;

//     for (int i = 0; i < 8; ++i)
//     {   
//         for (int j = 0; j < 2; ++j)
//         {
//             if (pos->piece_occupancy[i + 16*j] > -1)
//             {
//                 evaluation += (1 - 2*j)*value[i];
//             }

//             if (pos->piece_occupancy[i + 16*j + 8] > -1)
//             {
//                 switch (pos->promotions[i + 8*j])
//                 {
//                 case '\0':
//                     evaluation += (1 - 2*j);
//                     break;

//                 case 'q':
//                     evaluation += 9*(1 - 2*j);
//                     break;

//                 case 'n':
//                     evaluation += 3*(1 - 2*j);
//                     break;

//                 case 'r':
//                     evaluation += 5*(1 - 2*j);
//                     break;
                    
//                 case 'b':
//                     evaluation += 3*(1 - 2*j);
//                     break;

//                 default:
//                     break;
//                 }
//             }
//         }
//     }

//     pos->evaluation = evaluation;

//     std::cout << std::endl << "evaluation: " << evaluation << std::endl;
// }

void Engine::BuildTree(Tree& t)
{
    const int parity = (1 - t.position.turn)/2;
    board.GetMoves(t.position);

    for (int i = 0; i < 16; ++i)
    {
        Piece& p = t.position.pieces[i + 16*parity];
        size_t n_moves = p.moves.size();

        t.branches.resize(t.branches.size() + n_moves);
        t.alive.resize(t.alive.size() + n_moves, true);

        auto branch = t.branches.end() - n_moves;
        auto alive = t.alive.end() - n_moves;

        for (auto move = p.moves.begin(); move < p.moves.end(); ++move, ++branch, ++alive)
        {
            Position new_pos = Position(t.position);
            board.Move(p.id, *move, new_pos);

            if (transpose.find(new_pos) == transpose.end())
            {
                *branch = new Tree(new_pos, t.layer + 1);
                transpose[new_pos] = *branch;
            }

            else
            {
                *branch = transpose[new_pos];
                *alive = false;
            }
        }
    }

    // if (t.layer < depth)
    // {
    //     int n_branch = t.branches.size();

    //     for (int i = 0; i < n_branch; ++i)
    //     {
    //         if (t.alive[i])
    //         {
    //             BuildTree(*(t.branches[i]));
    //         }
    //     }
    // }
}

Engine::Engine(Board& b) : 
board(b)
{
    value[0] = 5;
    value[1] = 3;
    value[2] = 3;
    value[3] = 9;
    value[4] = 500;
    value[5] = 3;
    value[6] = 3;
    value[7] = 5;
}

Engine::~Engine()
{
    // delete tree;
}