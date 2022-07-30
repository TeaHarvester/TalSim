#include<iostream>
#include "Board.h"
#include "Piece.h"
#include "Helpers.h"

// bool Board::Check(Position*& pos)
// {
//     int king_id = 5 + (8*(1 - pos->turn));

//     for (int i = 0; i < 16; ++i)
//     {
//         Piece*& p = pieces[i + 8*(1 + pos->turn)];
//         GetMoves(p, pos);
//         int n_moves = p->moves.size();

//         for (int j = 0; j < n_moves; ++j)
//         {
//             if (pos->occupancy[p->moves[j]] == king_id)
//             {
//                 return true;
//             }
//         }
//     }

//     return false;
// }

void Board::Move(std::string move, char prom)
{
    current->GetMoves();

    char name = move[0];
    char disamb = '\0';
    int file;
    int rank;

    if (move == "00")
    {
        current->Move(5 + 8*(1 - current->turn), -1);
        return;
    }

    if (move == "000")
    {
        current->Move(5 + 8*(1 - current->turn), -2);
        return;
    }

    switch (move.size())
    {
    case 3:
        file = move[1] - 97;
        rank = move[2] - 49;
        break;

    case 4:
        disamb = move[1];
        file = move[2] - 97;
        rank = move[3] - 49;
        break;

    default:
        std::cout << std::endl << "invalid notation!" << std::endl;
        return;
    }

    int destination = 8*file + rank;
    int parity = (1 - current->turn)/2;

    std::vector<Piece*> candidates;

    for (int i = 16*parity; i < 16*(parity + 1); ++i)
    {
        Piece*& p = current->pieces[i];

        if (p && p->name == name)
        {
            std::vector<int>& moves = p->moves;

            int n_moves = p->moves.size();

            for (int j = 0; j < n_moves; ++j)
            {
                if (moves[j] == destination)
                {
                    candidates.push_back(p);
                }
            }
        }
    }

    int n_candidates = candidates.size();
    int id = 0;

    switch (n_candidates)
    {
    case 0:
        std::cout << std::endl << "illegal move!" << std::endl;
        return;

    case 1:
        id = candidates[0]->id;
        break;

    case 2:
        for (int i = 0; i < 2; ++i)
        {
            int y = candidates[i]->position % 8;
            int x = candidates[i]->position / 8;

            if ((disamb >= 97 && x == disamb - 97) || (disamb < 97 && y == disamb - 49))
            {
                id = candidates[i]->id;
            }
        }

        if (!id)
        {
            std::cout << std::endl << "can't disambiguate!" << std::endl;
            return;   
        }
    
    default:
        break;
    }

    current->Move(id, destination, prom);
}

// void Board::Move(int id, int destination, char prom)
// {
//     GetMoves(current);

//     Position* new_current = Move(id, destination, current, prom);

//     if (new_current)
//     {
//         delete current;
//         current = new_current;
//     }
// }

// Position* Board::Move(int id, int destination, Position*& pos, char prom)
// {
//     Piece*& p = pieces[id - 1];

//     int init_pos = p->position;

//     if (pos->kingsidecastling && destination == -1)
//     {
//         Position* new_pos = new Position(pos, id, init_pos + 16);

//         int rook_id = 8 + 8*(1 - pos->turn);

//         *new_pos = Position(new_pos, rook_id, pieces[rook_id - 1]->position - 16);

//         new_pos->kingsidecastling[(1 - pos->turn)/2] = false;
//         new_pos->queensidecastling[(1 - pos->turn)/2] = false;
//         new_pos->turn = -1*pos->turn;

//         return new_pos;
//     }

//     else if (pos->queensidecastling && destination == -2)
//     {
//         Position* new_pos = new Position(pos, id, init_pos - 16);

//         int rook_id = 1 + 8*(1 - pos->turn);

//         *new_pos = Position(new_pos, rook_id, pieces[rook_id - 1]->position + 24);

//         new_pos->kingsidecastling[(1 - pos->turn)/2] = false;
//         new_pos->queensidecastling[(1 - pos->turn)/2] = false;
//         new_pos->turn = -1*pos->turn;

//         return new_pos;
//     }

//     Position* new_pos = new Position(pos, id, destination, prom);

//     int captured = 0;

//     if (p->name == 'k')
//     {
//         new_pos->kingsidecastling[(1 - pos->turn)/2] = false;
//         new_pos->queensidecastling[(1 - pos->turn)/2] = false;
//     }

//     else if (id == 1 || id == 17)
//     {
//         new_pos->queensidecastling[(1 - pos->turn)/2] = false;
//     }

//     else if (id == 8 || id == 24)
//     {
//         new_pos->kingsidecastling[(1 - pos->turn)/2] = false;
//     }

//     else if (p->name == 'p')
//     {
//         if (pos->turn*(destination - init_pos) == 2)
//         {
//             new_pos->passant = destination - pos->turn;
//         }

//         else if ((destination - init_pos == -7*pos->turn || destination - init_pos == 9*pos->turn) && 
//                     destination == pos->passant)
//         {
//             captured = pos->occupancy[destination - pos->turn];
//             new_pos->occupancy[destination - pos->turn] = 0;
//             new_pos->piece_occupancy[captured - 1] = -1;
//         }
//     }

//     new_pos->turn = -1*pos->turn;

//     return new_pos;
// }

// void Board::Arrange(Position*& pos)
// {
//     for (int i = 0; i < 32; ++i)
//     {
//         Piece*& p = pieces[i];
//         int& position =  pos->piece_occupancy[i];

//         if (position > -1)
//         {
//             p->position = position;
//         }

//         if (((i >= 8 && i < 16) || (i >= 24 && i < 32)) && 
//             pos->promotions[p->colour == 1 ? i - 8 : i -16] != p->name)
//         {
//             switch (pos->promotions[p->colour == 1 ? i - 8 : i -16])
//             {
//             case 'q':
//                 *p = Queen(i + 1, position);
//                 break;

//             case 'n':
//                 *p = Knight(i + 1, position);
//                 break;

//             case 'b':
//                 *p = Bishop(i + 1, position);
//                 break;

//             case 'r':
//                 *p = Rook(i + 1, position);
//                 break;
            
//             default:
//                 *p = Pawn(i + 1, position);
//                 break;
//             }
//         }
//     }
// }

// void Board::TrimMoves(Position*& pos)
// {
//     Position* new_pos = new Position(pos);

//     for (int i = 0; i < 16; ++i)
//     {
//         Piece*& p = pieces[i + 8*(1 - pos->turn)];

//         auto iter = p->moves.begin();

//         while (iter != p->moves.end())
//         {
//             start:

//             int init_pos = p->position;
//             int destination = *iter;

//             if (pos->kingsidecastling && destination == -1)
//             {
//                 for (int i = 0; i < 3; ++i)
//                 {
//                     *new_pos = Position(pos, p->id, init_pos + 8*i);

//                     if (Check(new_pos))
//                     {
//                         iter = p->moves.erase(iter);
//                         goto start;
//                     }
//                 }
//             }

//             else if (pos->queensidecastling && destination == -2)
//             {
//                 for (int i = 0; i < 3; ++i)
//                 {
//                     *new_pos = Position(pos, p->id, init_pos - 8*i);

//                     if (Check(new_pos))
//                     {
//                         iter = p->moves.erase(iter);
//                         goto start;
//                     }
//                 }
//             }

//             else
//             {
//                 *new_pos = Position(pos, p->id, destination);

//                 if (Check(new_pos))
//                 {
//                     iter = p->moves.erase(iter);
//                     continue;
//                 }
//             } 

//             ++iter;
//         }
//     }

//     delete new_pos;
// }

// void Board::GetMoves(Position*& pos)
// {
//     Arrange(pos);

//     for (int i = 0; i < 32; ++i)
//     {   
//         Piece*& p = pieces[i];
//         GetMoves(p, pos);
//     }

//     TrimMoves(pos);
// }

// void Board::GetMoves(Piece*& p, Position*& pos)
// {   
//     p->moves.clear();

//     if (pos->piece_occupancy[p->id - 1] == -1)
//     {
//         return;
//     }

//     int pos_y = p->position % 8;
//     int pos_x = (p->position - pos_y)/8;

//     int n_vectors = p->vectors.size();

//     if (n_vectors > 1)
//     {
//         for (int i = 0; i < n_vectors; ++i)
//         {
//             for (int direction = -1; direction < 2; direction += 2)
//             {
//                 int x = pos_x;
//                 int y = pos_y;
//                 int x_movement = direction*p->vectors[i][1];
//                 int y_movement = direction*p->vectors[i][0];

//                 while (x + x_movement <= 7 && 
//                        y + y_movement <= 7 && 
//                        x + x_movement >= 0 && 
//                        y + y_movement >= 0)
//                 {
//                     x += x_movement;
//                     y += y_movement;

//                     if (pos->GetColour(8*x + y)*p->colour == 1)
//                     {
//                         break;
//                     }

//                     else if (pos->GetColour(8*x + y)*p->colour == -1)
//                     {
//                         p->moves.push_back(8*x + y);
//                         break;
//                     }

//                     p->moves.push_back(8*x + y);
//                 }
//             }
//         }
//     }

//     else if (p->name == 'p')
//     {
//         int x = pos_x;
//         int y = pos_y;

//         if (y + 2*p->colour <= 7 &&
//             y + 2*p->colour >= 0 && 
//             pos->occupancy[8*x + y + p->colour] == 0  &&
//             pos->occupancy[8*x + y + 2*p->colour] == 0 &&
//             !pos->moved[p->colour == 1 ? p->id - 9 : p->id -17])
//         {
//             p->moves.push_back(8*x + y + p->colour);
//             p->moves.push_back(8*x + y + 2*p->colour);
//         }

//         else if (y + p->colour <= 7 &&
//                  y + p->colour >= 0 && 
//                  pos->occupancy[8*x + y + p->colour] == 0)
//         {
//             p->moves.push_back(8*x + y + p->colour);
//         }

//         if (y + p->colour <= 7 &&
//             y + p->colour >= 0 &&
//             x + 1 <= 7 &&
//             (p->colour*pos->GetColour(8*(x + 1) + y + p->colour) == -1 || 8*(x + 1) + y + p->colour == pos->passant))
//         {
//             p->moves.push_back(8*(x + 1) + y + p->colour);
//         }

//         if (y + p->colour <= 7 &&
//             y + p->colour >= 0 &&
//             x - 1 >= 0 &&
//             (p->colour*pos->GetColour(8*(x - 1) + y + p->colour) == -1 || 8*(x - 1) + y + p->colour == pos->passant))
//         {
//             p->moves.push_back(8*(x - 1) + y + p->colour);
//         }
//     }

//     else
//     {
//         int x = pos_x;
//         int y = pos_y;

//         if (p->name == 'k' &&
//             pos->kingsidecastling[(1 - p->colour)/2] &&
//             !pos->occupancy[8*(x + 1) + y] && 
//             !pos->occupancy[8*(x + 2) + y])
//         {
//             p->moves.push_back(-1);
//         }

//         if (p->name == 'k' && 
//             pos->queensidecastling[(1 - p->colour)/2] && 
//             !pos->occupancy[8*(x - 1) + y] && 
//             !pos->occupancy[8*(x - 2) + y] && 
//             !pos->occupancy[8*(x - 3) + y])
//         {
//             p->moves.push_back(-2);
//         }

//         int n_hops = p->hops.size();

//         for (int i = 0; i < n_hops; ++i)
//         {
//             if (x + p->hops[i][1] <= 7 && 
//                 x + p->hops[i][1] >= 0 &&
//                 y + p->hops[i][0] <= 7 &&
//                 y + p->hops[i][0] >= 0 &&
//                 p->colour*pos->GetColour(8*(x + p->hops[i][1]) + y + p->hops[i][0]) <= 0)
//             {
//                 p->moves.push_back(8*(x + p->hops[i][1]) + y + p->hops[i][0]);
//             }
//         }
//     }
// }

void Board::PrintPosition(Position*& pos)
{
    std::cout << std::endl;

    for (int i = 7; i >= 0; --i)
    {
        std::cout << std::endl;

        for (int j = 0; j < 8; ++j)
        {
            if (!pos->occupancy[8*j + i])
            {
                std::cout << " . ";
            }

            else 
            {
                Piece*& p = current->pieces[pos->occupancy[8*j + i] - 1];
                char alignment = p->id > 16 ? 'b' : 'w';
                std::cout << alignment << p->name << ' ';
            }
        }      
    }
}

void Board::DisplayMoves(Position*& pos)
{
    current->GetMoves();
    int parity = (1 - current->turn)/2;

    std::cout << std::endl;

    for (int i = 16*parity; i < 16*(parity + 1); ++i)
    {
        Piece*& p = current->pieces[i];

        if (p)
        {   
            std::cout << std::endl;

            std::vector<int>& moves = p->moves;
            int n_moves = p->moves.size();

            if (!n_moves)
            {
                std::cout << "NULL";
                continue;
            }

            for (int j = 0; j < n_moves; ++j)
            {
                if (moves[j] == -1)
                {
                    std::cout << "O-O" << ' ';
                    continue;
                }

                else if (moves[j] == -2)
                {
                    std::cout << "OOO" << ' ';
                    continue;
                }

                std::cout << p->name << GetSquare(moves[j]) << ' ';
            }
        }
    }
}

Board::Board()
{
    current = new Position();

    // pieces = std::vector<Piece*>(32);

    // pieces[0] = new Rook(1, 0);
    // pieces[1] = new Knight(2, 8);
    // pieces[2] = new Bishop(3, 16);
    // pieces[3] = new Queen(4, 24);
    // pieces[4] = new King(5, 32);
    // pieces[5] = new Bishop(6, 40);
    // pieces[6] = new Knight(7, 48);
    // pieces[7] = new Rook(8, 56);

    // for (int i = 0; i < 8; ++i)
    // {
    //     pieces[8 + i] = new Pawn(i + 9, 8*i + 1);
    //     pieces[i + 24] = new Pawn(i + 25, 8*i + 6);
    // }

    // pieces[16] = new Rook(17, 7);
    // pieces[17] = new Knight(18, 15);
    // pieces[18] = new Bishop(19, 23);
    // pieces[19] = new Queen(20, 31);
    // pieces[20] = new King(21, 39);
    // pieces[21] = new Bishop(22, 47);
    // pieces[22] = new Knight(23, 55);
    // pieces[23] = new Rook(24, 63);
}

Board::~Board()
{
    delete current;

    // int n_pieces = pieces.size();

    // for (int i = 0; i < n_pieces; ++i)
    // {
    //     if (pieces[i])
    //     {
    //         delete pieces[i];
    //         pieces[i] = NULL;
    //     }
    // }
}