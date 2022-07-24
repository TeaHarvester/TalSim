#include<iostream>
#include "Board.h"
#include "Piece.h"
#include "Helpers.h"

bool Board::Check(int colour, Position*& pos)
{
    GetMoves(pos);
    int king_id = 5 + (8*(1 - colour));

    for (int i = 0; i < 16; ++i)
    {
        Piece*& p = pieces[i + 8*(1 + colour)];
        int n_moves = p->moves.size();

        for (int j = 1; j < n_moves; ++j)
        {
            if (pos->occupancy[p->moves[j]] == king_id)
            {
                return true;
            }
        }
    }

    return false;
}

void Board::Move(int id, int destination)
{
    Position* new_current = Move(id, destination, current);

    if (new_current)
    {
        delete current;
        current = new_current;
    }
}

Position* Board::Move(int id, int destination, Position*& pos)
{
    GetMoves(pieces[id - 1], pos);

    Piece*& p = pieces[id - 1];

    if (p->Legal(destination))
    {
        Position* new_pos = new Position(pos, id, destination);

        if (!Check(pos->turn, new_pos))
        {
            return new_pos;
        }
    }

    return NULL;
}

void Board::Arrange(Position*& pos)
{
    for (int i = 0; i < 32; ++i)
    {
        if (pos->piece_occupancy[i] > -1)
        {
            pieces[i]->position = pos->piece_occupancy[i];
        }
    }
}

void Board::GetMoves(Position*& pos)
{
    Arrange(pos);

    for (int i = 0; i < 16; ++i)
    {   
        int index = i + 8*(1 - pos->turn);
        Piece*& p = pieces[index];
        p->moves.clear();

        if (pos->piece_occupancy[index] >= 0)
        {
            GetMoves(p, pos);
        }
    }
}

void Board::GetMoves(Piece*& p, Position*& pos)
{   
    int pos_y = p->position % 8;
    int pos_x = (p->position - pos_y)/8;

    int n_vectors = p->vectors.size();

    if (n_vectors > 1)
    {
        for (int i = 0; i < n_vectors; ++i)
        {
            for (int direction = -1; direction < 2; direction += 2)
            {
                int x = pos_x;
                int y = pos_y;
                int x_movement = direction*p->vectors[i][1];
                int y_movement = direction*p->vectors[i][0];

                while (x + x_movement <= 7 && 
                       y + y_movement <= 7 && 
                       x + x_movement >= 0 && 
                       y + y_movement >= 0)
                {
                    x += x_movement;
                    y += y_movement;

                    if (pos->GetColour(8*x + y)*p->colour == 1)
                    {
                        break;
                    }

                    else if (pos->GetColour(8*x + y)*p->colour == -1)
                    {
                        p->moves.push_back(8*x + y);
                        break;
                    }

                    p->moves.push_back(8*x + y);
                }
            }
        }
    }

    else if (p->name == 'p')
    {
        int x = pos_x;
        int y = pos_y;

        if (y + 2*p->colour <= 7 &&
            y + 2*p->colour >= 0 && 
            pos->occupancy[8*x + y + p->colour] == 0  &&
            pos->occupancy[8*x + y + 2*p->colour] == 0 &&
            !pos->moved[p->colour == 1 ? p->id - 9 : p->id -17])
        {
            p->moves.push_back(8*x + y + p->colour);
            p->moves.push_back(8*x + y + 2*p->colour);
        }

        else if (y + p->colour <= 7 &&
                 y + p->colour >= 0 && 
                 pos->occupancy[8*x + y + p->colour] == 0)
        {
            p->moves.push_back(8*x + y + p->colour);
        }

        if (y + p->colour <= 7 &&
            y + p->colour >= 0 &&
            x + 1 <= 7 &&
            (p->colour*pos->GetColour(8*(x + 1) + y + p->colour) == -1 || 8*(x + 1) + y + p->colour == pos->passant))
        {
            p->moves.push_back(8*(x + 1) + y + p->colour);
        }

        if (y + p->colour <= 7 &&
            y + p->colour >= 0 &&
            x - 1 >= 0 &&
            (p->colour*pos->GetColour(8*(x - 1) + y + p->colour) == -1 || 8*(x - 1) + y + p->colour == pos->passant))
        {
            p->moves.push_back(8*(x - 1) + y + p->colour);
        }
    }

    else
    {
        int x = pos_x;
        int y = pos_y;

        if (p->name == 'k' &&
            pos->kingsidecastling[(1 - p->colour)/2] &&
            !pos->occupancy[8*(x + 1)] && 
            !pos->occupancy[8*(x + 2)])
        {
            p->moves.push_back(-1);
        }

        if (p->name == 'k' && 
            pos->queensidecastling[(1 - p->colour)/2] && 
            !pos->occupancy[8*(x - 1)] && 
            !pos->occupancy[8*(x - 2)] && 
            !pos->occupancy[8*(x - 3)])
        {
            p->moves.push_back(-2);
        }

        int n_hops = p->hops.size();

        for (int i = 0; i < n_hops; ++i)
        {
            if (x + p->hops[i][1] <= 7 && 
                x + p->hops[i][1] >= 0 &&
                y + p->hops[i][0] <= 7 &&
                y + p->hops[i][0] >= 0 &&
                p->colour*pos->GetColour(8*(x + p->hops[i][1]) + y + p->hops[i][0]) <= 0)
            {
                p->moves.push_back(8*(x + p->hops[i][1]) + y + p->hops[i][0]);
            }
        }
    }
}

void Board::PrintPosition(Position*& pos)
{
    std::cout << std::endl;

    for (int i = 0; i < 8; ++i)
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
                Piece*& p = pieces[pos->occupancy[8*j + i] - 1];
                char alignment = p->colour == 1 ? 'w' : 'b';
                std::cout << alignment << p->name << ' ';
            }
        }      
    }
}

void Board::DisplayMoves(Position*& pos)
{
    GetMoves(current);

    std::cout << std::endl;

    for (int i = 0; i < 16; ++i)
    {
        int index = i + 8*(1 - pos->turn);

        if (pieces[index])
        {   
            std::cout << std::endl;

            std::vector<int>& moves = pieces[index]->moves;

            int n_moves = moves.size();

            if (!n_moves)
            {
                std::cout << "NULL";
                continue;
            }

            for (int i = 0; i < n_moves; ++i)
            {
                std::cout << pieces[index]->name << GetSquare(moves[i]) << ' ';
            }
        }
    }
}

Board::Board()
{
    current = new Position();

    pieces = std::vector<Piece*>(32);

    pieces[0] = new Rook(1, 0);
    pieces[1] = new Knight(2, 8);
    pieces[2] = new Bishop(3, 16);
    pieces[3] = new Queen(4, 24);
    pieces[4] = new King(5, 32);
    pieces[5] = new Bishop(6, 40);
    pieces[6] = new Knight(7, 48);
    pieces[7] = new Rook(8, 56);

    for (int i = 0; i < 8; ++i)
    {
        pieces[8 + i] = new Pawn(i + 9, 8*i + 1);
    }

    pieces[16] = new Rook(17, 7);
    pieces[17] = new Knight(18, 15);
    pieces[18] = new Bishop(19, 23);
    pieces[19] = new Queen(20, 31);
    pieces[20] = new King(21, 39);
    pieces[21] = new Bishop(22, 47);
    pieces[22] = new Knight(23, 55);
    pieces[23] = new Rook(24, 63);

    for (int i = 0; i < 8; ++i)
    {
        pieces[i + 24] = new Pawn(i + 25, 8*i + 6);
    }
}

Board::~Board()
{
    delete current;

    int n_pieces = pieces.size();

    for (int i = 0; i < n_pieces; ++i)
    {
        if (pieces[i])
        {
            delete pieces[i];
            pieces[i] = NULL;
        }
    }
}