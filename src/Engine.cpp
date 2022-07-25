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

Engine::Engine()
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