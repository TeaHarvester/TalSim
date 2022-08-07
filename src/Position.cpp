#include "Position.h"
#include "Position.h"

Position::Position() :
turn(1),
occupancy{0},
passant(-1),
kingsidecastling{true, true},
queensidecastling{true, true},
control()
{
    for (int i = 0; i < 8; ++i)
    {
        occupancy[8*i] = i + 1;
        occupancy[8*i + 1] = i + 9;
        occupancy[8*i + 7] = i + 17;
        occupancy[8*i + 6] = i + 25;

        // piece_occupancy[i] = 8*i;
        // piece_occupancy[i + 8] = 8*i + 1;
        // piece_occupancy[i + 16] = 8*i + 7;
        // piece_occupancy[i + 24] = 8*i + 6;

        // promotions[i] = 'p';
        // promotions[i + 8] = 'p';
    }

    pieces[0] = Rook(1, 0);
    pieces[1] = Knight(2, 8);
    pieces[2] = Bishop(3, 16);
    pieces[3] = Queen(4, 24);
    pieces[4] = King(5, 32);
    pieces[5] = Bishop(6, 40);
    pieces[6] = Knight(7, 48);
    pieces[7] = Rook(8, 56);

    for (int i = 0; i < 8; ++i)
    {
        pieces[8 + i] = Pawn(i + 9, 8*i + 1);
    }

    pieces[16] = Rook(17, 7);
    pieces[17] = Knight(18, 15);
    pieces[18] = Bishop(19, 23);
    pieces[19] = Queen(20, 31);
    pieces[20] = King(21, 39);
    pieces[21] = Bishop(22, 47);
    pieces[22] = Knight(23, 55);
    pieces[23] = Rook(24, 63);

    for (int i = 0; i < 8; ++i)
    {
        pieces[i + 24] = Pawn(i + 25, 8*i + 6);
    }
}

Position::Position(const Position& pos) :
turn(pos.turn),
passant(pos.passant),
movelist(pos.movelist),
control(pos.control)
{
    for (int i = 0; i < 64; ++i)
    {
        occupancy[i] = pos.occupancy[i];

        // if (i < 32)
        // {
        //     piece_occupancy[i] = pos.piece_occupancy[i];

        //     if (i < 16)
        //     {
        //         promotions[i] = pos.promotions[i];
        //     }
        // }
    }

    kingsidecastling[0] = pos.kingsidecastling[0];
    kingsidecastling[1] = pos.kingsidecastling[1];
    queensidecastling[0] = pos.queensidecastling[0];
    queensidecastling[1] = pos.queensidecastling[1];

    for (int i = 0; i < 32; ++i)
    {
        pieces[i] = Piece(pos.pieces[i]);
    }


}

// Position::Position(const Position& pos, int id, int destination, char prom) : 
// turn(pos.turn),
// passant(-1)
// {
//     for (int i = 0; i < 64; ++i)
//     {
//         occupancy[i] = pos.occupancy[i];

//         if (i < 32)
//         {
//             piece_occupancy[i] = pos.piece_occupancy[i];

//             if (i < 16)
//             {
//                 promotions[i] = pos.promotions[i];
//             }
//         }
//     }

//     kingsidecastling[0] = pos.kingsidecastling[0];
//     kingsidecastling[1] = pos.kingsidecastling[1];
//     queensidecastling[0] = pos.queensidecastling[0];
//     queensidecastling[1] = pos.queensidecastling[1];

//     int captured = occupancy[destination];

//     if (captured > 0)
//     {
//         piece_occupancy[captured - 1] = -1;
//     }

//     occupancy[destination] = id;
//     occupancy[pos.piece_occupancy[id - 1]] = 0;
//     piece_occupancy[id - 1] = destination;

//     if ((id > 8 && id <= 16) || (id > 24 && id <= 32))
//     {
//         float promotion_rank = 3.5 + float(pos.turn)*3.5;
//         int index = (turn == 1 ? id - 9 : id -17);

//         if (destination % 8 == int(promotion_rank))
//         {
//             promotions[index] = prom;
//         }
//     }

//     pieces.resize(pos.pieces.size());

//     auto p = pos.pieces.begin();

//     for (auto p_copy = pieces.begin(); p_copy < pieces.end(); ++p_copy, ++p)
//     {
//         *p_copy = Piece(*p);
//     }
// }

bool Position::operator==(const Position& pos) const
{
    for (int i = 0; i < 64; ++i)
    {
        if (occupancy[i] != pos.occupancy[i])
        {
            return false;
        }
    }

    for (int i = 0; i < 8; ++i)
    {
        if (pieces[i + 8].name != pos.pieces[i + 8].name ||
            pieces[i + 24].name != pos.pieces[i + 24].name)
        {
            return false;   
        }
    }

    // for (int i = 0; i < 16; ++i)
    // {
    //     if (piece_occupancy[i] != pos.piece_occupancy[i] ||
    //         piece_occupancy[i + 16] != pos.piece_occupancy[i + 16] ||
    //         promotions[i] != pos.promotions[i])
    //     {
    //         return false;
    //     }
    // }

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