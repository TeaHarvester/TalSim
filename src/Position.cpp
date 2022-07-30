#include "Position.h"
#include "Position.h"
#include "Board.h"

int Position::Move(const int id, const int destination, const char prom)
{
    Piece*& p = pieces[id - 1];

    if (!p->Legal(destination))
    {
        return 0;
    }

    int init_pos = p->position;
    int parity = (1 - turn)/2;

    if (destination == -1)
    {
            pieces[4 + 16*parity]->position += 16;
            pieces[7 + 16*parity]->position -= 16;
            kingsidecastling[parity] = false;
            queensidecastling[parity] = false;
            occupancy[30 + 7*parity] = 0;
            occupancy[40 + 7*parity] = 8 + 16*parity;
            occupancy[48 + 7*parity] = 5 + 16*parity;
            occupancy[56 + 7*parity] = 0;
            turn = -1*turn;
            passant = -1;
            return 1;
    }

    if (destination == -2)
    {
        pieces[4 + 16*parity]->position -= 16;
        pieces[1 + 16*parity]->position += 24;
        kingsidecastling[parity] = false;
        queensidecastling[parity] = false;
        occupancy[7*parity] = 0;
        occupancy[8 + 7*parity] = 0;
        occupancy[16 + 7*parity] = 1 + 16*parity;
        occupancy[24 + 7*parity] = 5 + 16*parity;
        occupancy[30 + 7*parity] = 0;
        turn = -1*turn;
        passant = -1;
        return 1;
    }

    int captured = 0;

    if (p->name == 'k')
    {
        kingsidecastling[parity] = false;
        queensidecastling[parity] = false;
    }

    else if (id == 1 + 16*parity)
    {
        queensidecastling[parity] = false;
    }

    else if (id == 8 + 16*parity)
    {
        kingsidecastling[parity] = false;
    }

    else if (p->name == 'p')
    {
        if (turn*(destination - init_pos) == 2)
        {
            passant = destination - turn;
            occupancy[destination] = id;
            occupancy[destination - 2*turn] = 0;
            turn = -1*turn;
            pieces[id - 1]->position = destination;
            return 1;
        }

        else if (destination % 8 == 7*(1 - parity))
        {
            delete p;
            p = NULL;

            switch (prom)
            {
            case 'q':
                *p = Queen(id, destination);
                break;

            case 'n':
                *p = Knight(id, destination);
                break;

            case 'b':
                *p = Bishop(id, destination);
                break;

            case 'r':
                *p = Rook(id, destination);
                break;
            
            default:
                break;
            }
        }

        else if ((destination - init_pos == -7*turn || destination - init_pos == 9*turn) && 
                    destination == passant)
        {
            captured = occupancy[destination - turn];
            occupancy[destination] = id;
            occupancy[destination - turn] = 0;
            occupancy[init_pos] = 0;
            turn = -1*turn;
            passant = -1;
            pieces[id - 1]->position = destination;

            if (captured)
            {    
                delete pieces[captured - 1];
                pieces[captured - 1] = NULL;
            }

            return 1;
        }
    }

    captured = occupancy[destination];
    occupancy[destination] = id;
    occupancy[init_pos] = 0;
    turn = -1*turn;
    passant = -1;
    pieces[id - 1]->position = destination;

    if (captured)
    {    
        delete pieces[captured - 1];
        pieces[captured - 1] = NULL;
    }

    return 1;
}

void Position::GetMoves()
{
    for (int i = 0; i < 32; ++i)
    {   
        GetMoves(i);
    }

    TrimMoves();
}

void Position::GetMoves(const int index)
{
    Piece*& p = pieces[index];

    if (!p)
    {
        return;
    }

    int colour = p->id > 16 ? -1 : 1;

    p->moves.clear();

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

                    if (GetColour(8*x + y)*colour == 1)
                    {
                        break;
                    }

                    else if (GetColour(8*x + y)*colour == -1)
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
        int& x = pos_x;
        int& y = pos_y;

        if (y + 2*colour <= 7 &&
            y + 2*colour >= 0 && 
            occupancy[8*x + y + colour] == 0  &&
            occupancy[8*x + y + 2*colour] == 0 &&
            y == 1 + 5*(1 - colour)/2)
        {
            p->moves.push_back(8*x + y + colour);
            p->moves.push_back(8*x + y + 2*colour);
        }

        else if (y + colour <= 7 &&
                 y + colour >= 0 && 
                 occupancy[8*x + y + colour] == 0)
        {
            p->moves.push_back(8*x + y + colour);
        }

        if (y + colour <= 7 &&
            y + colour >= 0 &&
            x + 1 <= 7 &&
            (colour*GetColour(8*(x + 1) + y + colour) == -1 || 8*(x + 1) + y + colour == passant))
        {
            p->moves.push_back(8*(x + 1) + y + colour);
        }

        if (y + colour <= 7 &&
            y + colour >= 0 &&
            x - 1 >= 0 &&
            (colour*GetColour(8*(x - 1) + y + colour) == -1 || 8*(x - 1) + y + colour == passant))
        {
            p->moves.push_back(8*(x - 1) + y + colour);
        }
    }

    else
    {
        int x = pos_x;
        int y = pos_y;

        if (p->name == 'k' &&
            kingsidecastling[(1 - colour)/2] &&
            !occupancy[8*(x + 1) + y] && 
            !occupancy[8*(x + 2) + y])
        {
            p->moves.push_back(-1);
        }

        if (p->name == 'k' && 
            queensidecastling[(1 - colour)/2] && 
            !occupancy[8*(x - 1) + y] && 
            !occupancy[8*(x - 2) + y] && 
            !occupancy[8*(x - 3) + y])
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
                colour*GetColour(8*(x + p->hops[i][1]) + y + p->hops[i][0]) <= 0)
            {
                p->moves.push_back(8*(x + p->hops[i][1]) + y + p->hops[i][0]);
            }
        }
    }
}

void Position::TrimMoves()
{
    int parity = (1 - turn)/2;

    for (int i = 16*parity; i < 16*(parity + 1); ++i)
    {
        Piece*& p = pieces[i];

        if (!p)
        {
            continue;
        }

        auto iter = p->moves.begin();

        while (iter != p->moves.end())
        {
            int destination = *iter;

            if (destination == -1 && 
                kingsidecastling[parity] &&
                !occupancy[40 + 7*parity] &&
                !occupancy[48 + 7*parity])
            {
                for (int j = 16*(1 - parity); j < 16*(2 - parity); ++j)
                {
                    int n_moves = pieces[j]->moves.size();

                    for (int k = 0; k < n_moves; ++j)
                    {
                        int& move = pieces[j]->moves[k];

                        if (move == 4 + 7*parity || move == 5 + 7*parity || move == 6 + 7*parity)
                        {
                            iter = p->moves.erase(iter);
                            goto end;
                        }
                    }
                }
            }

            else if (destination == -2 &&
                     queensidecastling[parity] &&
                     !occupancy[8 + 7*parity] &&
                     !occupancy[16 + 7*parity] &&
                     !occupancy[24 + 7*parity])
            {
                for (int j = 16*(1 - parity); j < 16*(2 - parity); ++j)
                {
                    int n_moves = pieces[j]->moves.size();

                    for (int k = 0; k < n_moves; ++k)
                    {
                        int& move = pieces[j]->moves[k];

                        if (move == 4 + 7*parity || move == 3 + 7*parity || move == 2 + 7*parity)
                        {
                            iter = p->moves.erase(iter);
                            goto end;
                        }
                    }
                }
            }

            else
            {
                Position* new_pos = new Position(this);
                new_pos->Move(p->id, destination);

                if (new_pos->Check(turn))
                {
                    iter = p->moves.erase(iter);
                    delete new_pos;
                    goto end;
                }

                delete new_pos;
            }

            ++iter;

            end:;
        }
    }
}

bool Position::Check(int colour)
{
    int parity = (1 - colour)/2;
    Piece*& king = pieces[4 + 16*parity];
    int pos_y = king->position % 8;
    int pos_x = (king->position - pos_y)/8;

    int vectors[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    int hops[4][2] = {{-2, 1}, {-1, 2}, {1, 2}, {2, 1}};

    for (int i = 0; i < 4; ++i)
    {
        for (int direction = -1; direction < 2; direction += 2)
        {
            int x_hop = direction*hops[i][1];
            int y_hop = direction*hops[i][0];

            if (pos_x + x_hop <= 7 && 
                pos_y + y_hop <= 7 && 
                pos_x + x_hop >= 0 && 
                pos_y + y_hop >= 0)
            {
                int leap = 8*(pos_x + x_hop) + pos_y + y_hop;

                if (colour*GetColour(leap) == -1 && GetAggressor(leap, -1*colour) == 'n')
                {
                    return true;
                }
            }

            int x = pos_x;
            int y = pos_y;
            int x_movement = direction*vectors[i][1];
            int y_movement = direction*vectors[i][0];

            while (x + x_movement <= 7 && 
                    y + y_movement <= 7 && 
                    x + x_movement >= 0 && 
                    y + y_movement >= 0)
            {
                x += x_movement;
                y += y_movement;

                if (GetColour(8*x + y)*colour == 1)
                {
                    break;
                }

                else if (GetColour(8*x + y)*colour == -1)
                {
                    char aggressor = GetAggressor(8*x + y, -1*colour);

                    if (aggressor == 'q' ||
                        ((aggressor == 'r') && (!vectors[i][0] || !vectors[i][1])) ||
                        ((aggressor == 'b') && (vectors[i][0] && vectors[i][1])))
                    {
                        return true;
                    }
                }
            }
        }
    }

    Piece*& opposing_king = pieces[4 + 16*(1 - parity)];
    int op_y = opposing_king->position % 8;
    int op_x = (opposing_king->position - op_y)/8;

    int sep_x = pos_x - op_x;
    int sep_y = pos_y - op_y;

    if ((sep_x == 1 || sep_x == -1) &&
        (sep_y == 1 || sep_y == -1))
        {
            return true;
        }

    return false;
}

char Position::GetAggressor(const int square, const int colour)
{
    int parity = (1 - colour)/2;

    for (int i = 0; i < 16; ++i)
    {
        Piece*& p = pieces[i + 16*parity];

        if (p && p->position == square)
        {
            return p->name;
        }
    }

    return '\0';
}

int Position::GetColour(int square)
{
    if (!occupancy[square])
    {
        return 0;
    }

    return occupancy[square] <= 16 ? 1 : -1; 
}

Position::Position(const Position* pos) :
turn(pos->turn),
passant(pos->passant)
{
    for (int i = 0; i < 64; ++i)
    {
        occupancy[i] = pos->occupancy[i];

        if (i < 32)
        {   
            if (pos->pieces[i])
            {
                pieces[i] = pos->pieces[i]->Copy();
            }

            else
            {
                pieces[i] = NULL;
            }
        }
    }

    kingsidecastling[0] = pos->kingsidecastling[0];
    kingsidecastling[1] = pos->kingsidecastling[1];
    queensidecastling[0] = pos->queensidecastling[0];
    queensidecastling[1] = pos->queensidecastling[1];
}

Position::Position() :
turn(1),
occupancy{0},
passant(-1),
kingsidecastling{true, true},
queensidecastling{true, true},
pieces{NULL}
{
    pieces[0] = new Rook(1, 0);
    pieces[1] = new Knight(2, 8);
    pieces[2] = new Bishop(3, 16);
    pieces[3] = new Queen(4, 24);
    pieces[4] = new King(5, 32);
    pieces[5] = new Bishop(6, 40);
    pieces[6] = new Knight(7, 48);
    pieces[7] = new Rook(8, 56);
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
        occupancy[8*i] = i + 1;
        occupancy[8*i + 1] = i + 9;
        occupancy[8*i + 7] = i + 17;
        occupancy[8*i + 6] = i + 25;

        pieces[8 + i] = new Pawn(i + 9, 8*i + 1);
        pieces[i + 24] = new Pawn(i + 25, 8*i + 6);
    }
}

Position::~Position()
{
    for (int i = 0; i < 32; ++i)
    {
        if (&pieces[i])
        {
            delete pieces[i];
        }
    }
}