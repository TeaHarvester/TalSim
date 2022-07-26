#include "Position.h"
#include "Position.h"

Position::Position() :
turn(1),
occupancy{0},
passant(-1),
kingsidecastling{true, true},
queensidecastling{true, true}
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

        promotions[i] = 'p';
        promotions[i + 8] = 'p';
    }
}

Position::Position(const Position& pos) :
turn(pos.turn),
passant(pos.passant)
{
    for (int i = 0; i < 64; ++i)
    {
        occupancy[i] = pos.occupancy[i];

        if (i < 32)
        {
            piece_occupancy[i] = pos.piece_occupancy[i];

            if (i < 16)
            {
                promotions[i] = pos.promotions[i];
            }
        }
    }

    kingsidecastling[0] = pos.kingsidecastling[0];
    kingsidecastling[1] = pos.kingsidecastling[1];
    queensidecastling[0] = pos.queensidecastling[0];
    queensidecastling[1] = pos.queensidecastling[1];
}

Position::Position(const Position& pos, int id, int destination, char prom) : 
turn(pos.turn),
passant(-1)
{
    for (int i = 0; i < 64; ++i)
    {
        occupancy[i] = pos.occupancy[i];

        if (i < 32)
        {
            piece_occupancy[i] = pos.piece_occupancy[i];

            if (i < 16)
            {
                promotions[i] = pos.promotions[i];
            }
        }
    }

    kingsidecastling[0] = pos.kingsidecastling[0];
    kingsidecastling[1] = pos.kingsidecastling[1];
    queensidecastling[0] = pos.queensidecastling[0];
    queensidecastling[1] = pos.queensidecastling[1];

    int captured = occupancy[destination];

    if (captured > 0)
    {
        piece_occupancy[captured - 1] = -1;
    }

    occupancy[destination] = id;
    occupancy[pos.piece_occupancy[id - 1]] = 0;
    piece_occupancy[id - 1] = destination;

    if ((id > 8 && id <= 16) || (id > 24 && id <= 32))
    {
        float promotion_rank = 3.5 + float(pos.turn)*3.5;
        int index = (turn == 1 ? id - 9 : id -17);

        if (destination % 8 == int(promotion_rank))
        {
            promotions[index] = prom;
        }
    }
}

bool Position::operator==(const Position& pos) const
{
    for (int i = 0; i < 16; ++i)
    {
        if (piece_occupancy[i] != pos.piece_occupancy[i] ||
            piece_occupancy[i + 16] != pos.piece_occupancy[i + 16] ||
            promotions[i] != pos.promotions[i])
        {
            return false;
        }
    }

    if (kingsidecastling[0] != pos.kingsidecastling[0] ||
    kingsidecastling[1] != pos.kingsidecastling[1] ||
    queensidecastling[0] != pos.queensidecastling[0] ||
    queensidecastling[1] != pos.queensidecastling[1] ||
    passant != pos.passant || turn != pos.turn)
    {
        return false;
    }

    return true;
}