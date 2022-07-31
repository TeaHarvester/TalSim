#include <chrono>
#include "Engine.h"

void Engine::Evaluate(Position*& pos)
{
    float evaluation = 0;

    for (int i = 0; i < 8; ++i)
    {   
        for (int j = 0; j < 2; ++j)
        {
            if (pos->piece_occupancy[i + 16*j] > -1)
            {
                evaluation += (1 - 2*j)*value[i];
            }

            if (pos->piece_occupancy[i + 16*j + 8] > -1)
            {
                switch (pos->promotions[i + 8*j])
                {
                case '\0':
                    evaluation += (1 - 2*j);
                    break;

                case 'q':
                    evaluation += 9*(1 - 2*j);
                    break;

                case 'n':
                    evaluation += 3*(1 - 2*j);
                    break;

                case 'r':
                    evaluation += 5*(1 - 2*j);
                    break;
                    
                case 'b':
                    evaluation += 3*(1 - 2*j);
                    break;

                default:
                    break;
                }
            }
        }
    }

    pos->evaluation = evaluation;

    std::cout << std::endl << "evaluation: " << evaluation << std::endl;
}

void Engine::BuildTree(const int depth, Tree*& t)
{
    board.GetMoves(t->position);

    for (int i = 0; i < 16; ++i)
    {
        Piece*& p = board.pieces[i + 8*(1 - t->position->turn)];
        int n_moves = p->moves.size();
        t->branches.resize(t->branches.size() + n_moves);
        auto branch = t->branches.end() - n_moves;

        for (int j = 0; j < n_moves; ++j)
        {
            Position* new_pos = board.Move(p->id, p->moves[j], t->position);
            *branch = new Tree(new_pos, t->layer + 1);
            t->n_branch += 1;
        }
    }

    if (t->layer < depth)
    {
        int n_branch = t->branches.size();

        for (int i = 0; i < n_branch; ++i)
        {
            BuildTree(depth, t->branches[i]);
        }
    }
}

Engine::Engine(Board& b) : 
tree(NULL),
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
    delete tree;
}