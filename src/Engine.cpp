#include "Engine.h"

void Engine::Evaluate(Position*& pos)
{
    float evaluation = 0;

    for (int i = 0; i < 8; ++i)
    {   
        for (int j = 0; j < 2; ++j)
        {
            if (pos->pieces[i + 16*j])
            {
                evaluation += (1 - 2*j)*value[i];
            }

            if (pos->pieces[i + 16*j + 8])
            {
                switch (pos->pieces[i + 16*j + 8]->name)
                {
                case 'p':
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
    t->position->GetMoves();
    int parity = (1 - t->position->turn)/2;

    for (int i = 16*parity; i < 16*(1 - parity); ++i)
    {
        Piece*& p = t->position->pieces[parity];
        int n_moves = p->moves.size();

        for (int j = 0; j < n_moves; ++j)
        {
            Position* new_pos = new Position(t->position);
            t->Branch(new_pos);
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