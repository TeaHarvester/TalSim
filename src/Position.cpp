#include "Position.h"
#include "Position.h"

int Position::GetColour(int square)
{
    if (!occupancy[square])
    {
        return 0;
    }

    return occupancy[square] <= 16 ? 1 : -1; 
}

Position::Position() :
turn(1),
occupancy{0},
piece_occupancy{-1},
moved{false},
kingsidecastling{true},
queensidecastling{true},
passant(false)
{
    for (int i = 0; i < 8; ++i)
    {
        occupancy[8*i] = i + 1;
        occupancy[8*i + 1] = i + 9;
        occupancy[8*i + 7] = i + 17;
        occupancy[8*i + 6] = i + 25;
        piece_occupancy[i] = 8*i;
        piece_occupancy[i + 8] = 8*i + 1;
        piece_occupancy[i + 16] = 8*i + 7;
        piece_occupancy[i + 24] = 8*i + 6;
    }
}

Position::Position(const Position* pos, int id, int destination) : 
turn(-1*pos->turn),
passant(false)
{
    for (int i = 0; i < 64; ++i)
    {
        occupancy[i] = pos->occupancy[i];

        if (i < 32)
        {
            piece_occupancy[i] = pos->piece_occupancy[i];

            if (i < 16)
            {
                moved[i] = pos->moved[i];
            }
        }
    }

    int captured = occupancy[destination];

    if (captured > 0)
    {
        piece_occupancy[captured - 1] = -1;
    }

    occupancy[destination] = id;
    occupancy[pos->piece_occupancy[id - 1]] = 0;
    piece_occupancy[id - 1] = destination;

    if ((id > 8 && id <= 16) || (id > 24 && id <= 32))
    {
        int index = id > 16? id - 17 : id - 9;
        moved[index] = true;

        if (pos->piece_occupancy[id - 1] == destination - 2*pos->turn)
        {
            passant = destination - pos->turn;
        }
    }

    if (id == 5 || id == 21)
    {
        kingsidecastling[(1 - pos->turn)/2] = false;
        queensidecastling[(1 - pos->turn)/2] = false;
    }

    else if (id == 1 || id == 17)
    {
        queensidecastling[(1 - pos->turn)/2] = false;
    }

    else if (id == 8 || id == 24)
    {
        kingsidecastling[(1 - pos->turn)/2] = false;
    }
}