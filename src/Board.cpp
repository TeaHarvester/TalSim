#include<iostream>
#include "tgmath.h"
#include "Board.h"
#include "Piece.h"
#include "Helpers.h"

int Board::GetColour(Position& pos, const int square)
{
    if (!pos.occupancy[square])
    {
        return 0;
    }

    return pos.occupancy[square] <= 16 ? 1 : -1; 
}

void Board::Move(const std::string move, const char prom)
{
    const int parity = (1 - current.turn)/2;

    GetMoves(current);

    char name = move[0];
    char disamb = '\0';
    int file;
    int rank;

    if (move == "00")
    {
        Move(5 + 16*parity, -1);
        return;
    }

    if (move == "000")
    {
        Move(5 + 16*parity, -2);
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

    std::vector<Piece> candidates;

    for (int i = 16*parity; i < 16*(parity + 1); ++i)
    {
        Piece& p = current.pieces[i];

        if (p.awake && p.name == name)
        {
            std::vector<int>& moves = p.moves;

            for (auto move = moves.begin(); move < moves.end(); ++move)
            {
                if (*move == destination)
                {
                    candidates.push_back(p);
                }
            }
        }
    }

    size_t n_candidates = candidates.size();
    int id = 0;

    switch (n_candidates)
    {
    case 0:
        std::cout << std::endl << "illegal move!" << std::endl;
        return;

    case 1:
        id = candidates[0].id;
        break;

    case 2:
        for (int i = 0; i < 2; ++i)
        {
            int y = candidates[i].position % 8;
            int x = candidates[i].position / 8;

            if ((disamb >= 97 && x == disamb - 97) || (disamb < 97 && y == disamb - 49))
            {
                id = candidates[i].id;
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

    Move(id, destination, prom);
}

void Board::Move(const int id, const int destination, const char prom)
{
    GetMoves(current);
    Move(id, destination, current, prom);
}

void Board::Move(const int id, const int destination, Position& pos, const char prom)
{
    Piece& p = pos.pieces[id - 1];
    const int parity = (1 - pos.turn)/2;
    int init_pos = p.position;
    int captured;
    pos.control[parity].clear();

    if (pos.kingsidecastling && destination == -1)
    {
        int rook_id = 8 + 16*parity;
        pos.pieces[rook_id - 1].position -= 16;
        p.position += 16;
        pos.occupancy[56 + 8*parity] = 0;
        pos.occupancy[32 + 8*parity] = 0;
        pos.occupancy[40 + 8*parity] = rook_id;
        pos.occupancy[48 + 8*parity] = p.id;
        pos.kingsidecastling[parity] = false;
        pos.queensidecastling[parity] = false;

        goto end;
    }

    else if (pos.queensidecastling && destination == -2)
    {

        int rook_id = 1 + 16*parity;
        pos.pieces[rook_id - 1].position += 24;
        p.position -= 16;
        pos.occupancy[0 + 8*parity] = 0;
        pos.occupancy[32 + 8*parity] = 0;
        pos.occupancy[16 + 8*parity] = rook_id;
        pos.occupancy[24 + 8*parity] = p.id;
        pos.kingsidecastling[parity] = false;
        pos.queensidecastling[parity] = false;

        goto end;
    }

    switch (p.name)
    {
        case 'q':
            break;

        case 'b':
            break;

        case 'r':
            if (p.id == 1 + 16*parity)
            {
                pos.queensidecastling[parity] = false;
            }

            else
            {
                pos.kingsidecastling[parity] = false;
            }

            break;

        case 'n':
            break;

        case 'k':
            pos.kingsidecastling[parity] = false;
            pos.queensidecastling[parity] = false;
            break;

        case 'p':
            if (pos.turn*(destination - init_pos) == 2)
            {
                pos.passant = destination - pos.turn;
            }

            else if ((destination - init_pos == -7*pos.turn || destination - init_pos == 9*pos.turn) && 
                      destination == pos.passant)
            {
                int captured = pos.occupancy[destination - pos.turn];
                pos.pieces[captured - 1].awake = false;
                pos.occupancy[destination - pos.turn] = 0;
            }
            break;

        default:
            break;
    }

    captured = pos.occupancy[destination];

    if (captured > 0)
    {
        pos.pieces[captured - 1].awake = false;
    }

    pos.occupancy[destination] = id;
    pos.occupancy[p.position] = 0;
    p.position = destination;

    if (id > 8 + 16*parity && id <= 16*(parity + 1))
    {
        int promotion_rank = parity ? 0 : 7;

        if (destination % 8 == promotion_rank && p.name == 'p')
        {
            switch (prom)
            {
            case 'q':
                p = Queen(p.id, p.position);
                break;

            case 'n':
                p = Knight(p.id, p.position);
                break;

            case 'b':
                p = Bishop(p.id, p.position);
                break;

            case 'r':
                p = Rook(p.id, p.position);
                break;
            
            default:
                break;
            }
        }
    }

    end:

    for (int i = 16*parity; i < 16*(1 + parity); ++i)
    {
        Piece& p = pos.pieces[i];

        switch (p.name)
        {
        case 'q':
        GetVectorControl(p, pos);
            break;

        case 'b':
        GetVectorControl(p, pos);
            break;

        case 'r':
        GetVectorControl(p, pos);
            break;

        case 'n':
        GetHopControl(p, pos);
            break;

        case 'k':
        GetHopControl(p, pos);
            break;
    
        case 'p':
        GetPawnControl(p, pos);
            break;

        default:
            break;
        }
    }

    pos.turn = -1*pos.turn;
    pos.movelist.push_back({id, destination});
}

void Board::GetPinVectors(Position& pos)
{
    const int parity = (1 - pos.turn)/2;
    Piece& king = pos.pieces[4 + 16*parity];

    const int pos_y = king.position % 8;
    const int pos_x = (king.position - pos_y)/8;
    const int n_vectors = 2;

    for (int i = 0; i < n_vectors; ++i)
    {
        for (int direction = -1; direction < 2; direction += 2)
        {
            int x = pos_x;
            int y = pos_y;
            const int x_movement = direction*Bishop::vectors[i][1];
            const int y_movement = direction*Bishop::vectors[i][0];
            int pin = -1;

            while (x + x_movement <= 7 && 
                    y + y_movement <= 7 && 
                    x + x_movement >= 0 && 
                    y + y_movement >= 0)
            {
                x += x_movement;
                y += y_movement;

                if (GetColour(pos, 8*x + y)*king.colour == 1)
                {
                    if (pin >= 0)
                    {
                        break;
                    }

                    else
                    {
                        pin = 8*x + y;
                    }
                }

                else if (GetColour(pos, 8*x + y)*king.colour == -1)
                {
                    if (!(pos.pieces[pos.occupancy[8*x + y] - 1].name == 'b' || 
                        pos.pieces[pos.occupancy[8*x + y] - 1].name == 'q'))
                        {
                            break;
                        }

                    if (pin >= 0)
                    {
                        pos.pieces[pos.occupancy[pin] - 1].pin_vector = 8*x_movement + y_movement;
                    }

                    break;
                }
            }
        }
    }

    for (int i = 0; i < n_vectors; ++i)
    {
        for (int direction = -1; direction < 2; direction += 2)
        {
            int x = pos_x;
            int y = pos_y;
            const int x_movement = direction*Rook::vectors[i][1];
            const int y_movement = direction*Rook::vectors[i][0];
            int pin = -1;

            while (x + x_movement <= 7 && 
                    y + y_movement <= 7 && 
                    x + x_movement >= 0 && 
                    y + y_movement >= 0)
            {
                x += x_movement;
                y += y_movement;

                if (GetColour(pos, 8*x + y)*king.colour == 1)
                {
                    if (pin >= 0)
                    {
                        break;
                    }

                    else
                    {
                        pin = 8*x + y;
                    }
                }

                else if (GetColour(pos, 8*x + y)*king.colour == -1)
                {
                    if  (!(pos.pieces[pos.occupancy[8*x + y] - 1].name == 'r' || 
                        pos.pieces[pos.occupancy[8*x + y] - 1].name == 'q'))
                    {
                        break;
                    }

                    if (pin >= 0)
                    {
                        pos.pieces[pos.occupancy[pin] - 1].pin_vector = 8*x_movement + y_movement;
                    }

                    break;
                }
            }
        }
    }
}

void Board::GetMoves(Position& pos)
{
    const int parity = (1 - pos.turn)/2;

    Piece& op_king = pos.pieces[4 + 16*(1 - parity)];
    Piece& king = pos.pieces[4 + 16*(parity)];
    size_t checks = king.check_vectors.size();
    std::vector<int>* legal_moves = NULL;

    op_king.check_vectors.clear();
    GetPinVectors(pos);

    for (int i = 16*parity; i < 16*(parity + 1); ++i)
    {
        pos.pieces[i].moves.clear();
        pos.pieces[i].pin_vector = -1;
    }

    if (checks > 0)
    {
        if (checks > 1)
        {
            GetKingMoves(king, pos);
            return;
        }

        else 
        {
            const int king_y = king.position % 8; 
            const int king_x = (king.position - king_y)/8; 
            const int aggressor_y = king.check_vectors[0] % 8;   
            const int aggressor_x = (king.check_vectors[0] - aggressor_y)/8; 
            legal_moves = new std::vector<int>;
            char& aggressor = pos.pieces[pos.occupancy[king.check_vectors[0]] - 1].name;

            if (aggressor == 'q' || aggressor == 'b' || aggressor == 'r' )
            {
                const int vec_y = aggressor_y - king_y;
                const int vec_x = aggressor_x - king_x;
                const int incr_x = (vec_x > 0) - (vec_x < 0);
                const int incr_y = (vec_y > 0) - (vec_y < 0);
                int x = king_x + incr_x;
                int y = king_y + incr_y;

                while (x <= aggressor_x && y <= aggressor_y)
                {
                    legal_moves->push_back(8*x + y);
                    x += incr_x;
                    y += incr_y;
                }
            }

            else 
            {
                legal_moves->push_back(8*aggressor_x + aggressor_y);
            }
        }
    }


    for (int i = 16*parity; i < 16*(parity + 1); ++i)
    {
        Piece& p = pos.pieces[i];

        if (!p.awake)
        {
            continue; 
        }

        switch (p.name)
        {
        case 'q':
        GetVectorMoves(p, pos, legal_moves);
            break;

        case 'b':
        GetVectorMoves(p, pos, legal_moves);
            break;

        case 'r':
        GetVectorMoves(p, pos, legal_moves);
            break;

        case 'n':
        GetKnightMoves(p, pos, legal_moves);
            break;

        case 'k':
        GetKingMoves(p, pos);
            break;
    
        case 'p':
        GetPawnMoves(p, pos, legal_moves);
            break;

        default:
            break;
        }
    }

    if (legal_moves)
    {
        delete legal_moves;
    }
}

void Board::GetVectorControl(Piece& p, Position& pos)
{
    const int parity = (1 - p.colour)/2;
    const int pos_y = p.position % 8;
    const int pos_x = (p.position - pos_y)/8;
    std::vector<std::vector<int>> vectors;

    switch (p.name)
    {
    case 'q':
        vectors = Queen::vectors;
        break;

    case 'b':
        vectors = Bishop::vectors;
        break;

    case 'r':
        vectors = Rook::vectors;
        break;

    default:
        break;
    }

    Piece& op_king = pos.pieces[4 + 16*(1 - parity)];

    for (auto vector = vectors.begin(); vector < vectors.end(); ++vector)
    {
        for (int direction = -1; direction < 2; direction += 2)
        {
            int x = pos_x;
            int y = pos_y;
            const int x_movement = direction*(*vector)[1];
            const int y_movement = direction*(*vector)[0];

            while (x + x_movement <= 7 && 
                    y + y_movement <= 7 && 
                    x + x_movement >= 0 && 
                    y + y_movement >= 0)
            {
                x += x_movement;
                y += y_movement;

                int square = 8*x + y;
                pos.control[parity][square].push_back(p.id);

                if (pos.occupancy[square])
                {
                    if (square == op_king.position)
                    {
                        op_king.check_vectors.push_back(p.position);
                    }

                    break;
                }
            }
        }
    }
}

void Board::GetHopControl(Piece& p, Position& pos)
{
    const int parity = (1 - p.colour)/2;
    const int pos_y = p.position % 8;
    const int pos_x = (p.position - pos_y)/8;
    std::vector<std::vector<int>> vectors;

    switch (p.name)
    {
    case 'n':
        vectors = Knight::vectors;
        break;

    case 'k':
        vectors = King::vectors;
        break;

    default:
        break;
    }

    Piece& op_king = pos.pieces[4 + 16*(1 - parity)];

    for (auto vector = vectors.begin(); vector < vectors.end(); ++vector)
    {
        for (int direction = -1; direction < 2; direction += 2)
        {
            int x = pos_x;
            int y = pos_y;
            const int x_movement = direction*(*vector)[1];
            const int y_movement = direction*(*vector)[0];

            if (x + x_movement <= 7 && 
                y + y_movement <= 7 && 
                x + x_movement >= 0 && 
                y + y_movement >= 0)
            {
                x += x_movement;
                y += y_movement;

                int square = 8*x + y;
                pos.control[parity][square].push_back(p.id);

                if (square == op_king.position)
                {
                    op_king.check_vectors.push_back(p.position);
                }
            }
        }
    }
}

void Board::GetPawnControl(Piece& p, Position& pos)
{
    const int parity = (1 - p.colour)/2;
    const int y = p.position % 8;
    const int x = (p.position - y)/8;
    Piece& op_king = pos.pieces[4 + 16*(1 - parity)];

    if (y + p.colour <= 7 &&
        y + p.colour >= 0 &&
        x + 1 <= 7)
    {
        int square = 8*(x + 1) + y + p.colour;
        pos.control[parity][square].push_back(p.id);

        if (square == op_king.position)
        {
            op_king.check_vectors.push_back(p.position);
        }
    }

    if (y + p.colour <= 7 &&
        y + p.colour >= 0 &&
        x - 1 >= 0)
    {
        int square = 8*(x - 1) + y + p.colour;
        pos.control[parity][square].push_back(p.id);

        if (square == op_king.position)
        {
            op_king.check_vectors.push_back(p.position);
        }
    }
}

void Board::GetVectorMoves(Piece& p, Position& pos, std::vector<int>* legal_moves)
{   
    std::vector<std::vector<int>> vectors;

    switch (p.name)
    {
    case 'q':
        vectors = Queen::vectors;
        break;

    case 'b':
        vectors = Bishop::vectors;
        break;

    case 'r':
        vectors = Rook::vectors;
        break;

    default:
        break;
    }

    if (legal_moves)
    {
        const int pos_y = p.position % 8;
        const int pos_x = (p.position - pos_y)/8;

        for (auto vector = vectors.begin(); vector < vectors.end(); ++vector)
        {
            for (int direction = -1; direction < 2; direction += 2)
            {
                int x = pos_x;
                int y = pos_y;
                const int x_movement = direction*(*vector)[1];
                const int y_movement = direction*(*vector)[0];

                while (x + x_movement <= 7 && 
                       y + y_movement <= 7 && 
                       x + x_movement >= 0 && 
                       y + y_movement >= 0)
                {
                    x += x_movement;
                    y += y_movement;
                    int square = 8*x + y;

                if (GetColour(pos, square)*p.colour == 1)
                {
                    break;
                }

                    for (auto legal = legal_moves->begin(); legal < legal_moves->end(); ++legal)
                    {
                        if (square == *legal)
                        {
                            p.moves.push_back(square);
                            continue;
                        }
                    }
                }
            }
        }

        return;
    }

    if (p.pin_vector >= 0)
    {
        bool stuck = true;

        for (auto vector = vectors.begin(); vector < vectors.end(); ++vector)
        {
            if (8*(*vector)[1] + (*vector)[0] == p.pin_vector ||
                -8*(*vector)[1] - (*vector)[0] == p.pin_vector)
            {
                stuck = false;
                break;
            }
        }

        if (stuck)
        {
            return;
        }

        int square = p.position;

        for (int i = 0; i < 7; ++i)
        {
            square += p.pin_vector;
            p.moves.push_back(square);

            if (pos.occupancy[square])
            {
                break;
            }
        }

        square = p.position;

        for (int i = 0; i < 7; ++i)
        {
            square -= p.pin_vector;

            if (pos.occupancy[square])
            {
                break;
            }

            p.moves.push_back(square);
        }

        return;
    }

    const int pos_y = p.position % 8;
    const int pos_x = (p.position - pos_y)/8;

    for (auto vector = vectors.begin(); vector < vectors.end(); ++vector)
    {
        for (int direction = -1; direction < 2; direction += 2)
        {
            int x = pos_x;
            int y = pos_y;
            const int x_movement = direction*(*vector)[1];
            const int y_movement = direction*(*vector)[0];

            while (x + x_movement <= 7 && 
                    y + y_movement <= 7 && 
                    x + x_movement >= 0 && 
                    y + y_movement >= 0)
            {
                x += x_movement;
                y += y_movement;
                const int square = 8*x + y;

                if (GetColour(pos, square)*p.colour == 1)
                {
                    break;
                }

                else if (GetColour(pos, square)*p.colour == -1)
                {
                    p.moves.push_back(square);
                    break;
                }

                p.moves.push_back(square);
            }
        }
    }
}

void Board::GetKnightMoves(Piece& p, Position& pos, std::vector<int>* legal_moves)
{
    const std::vector<std::vector<int>>& vectors = Knight::vectors;

    if (p.pin_vector > -1)
    {
        return;
    }

    if (legal_moves)
    {
        const int pos_y = p.position % 8;
        const int pos_x = (p.position - pos_y)/8;

        for (auto vector = vectors.begin(); vector < vectors.end(); ++vector)
        {
            for (int direction = -1; direction < 2; direction += 2)
            {
                const int x_movement = direction*(*vector)[1];
                const int y_movement = direction*(*vector)[0];

                if (pos_x + x_movement <= 7 && 
                    pos_y + y_movement <= 7 && 
                    pos_x + x_movement >= 0 && 
                    pos_y + y_movement >= 0)
                {
                    const int square = 8*(pos_x + x_movement) + pos_y + y_movement;

                    for (auto legal = legal_moves->begin(); legal < legal_moves->end(); ++legal)
                    {
                        if (square == *legal)
                        {
                            p.moves.push_back(square);
                            continue;
                        }
                    }
                }
            }
        }

        return;
    }

    const int pos_y = p.position % 8;
    const int pos_x = (p.position - pos_y)/8;

    for (auto vector = vectors.begin(); vector < vectors.end(); ++vector)
    {
        for (int direction = -1; direction < 2; direction += 2)
        {
            const int x_movement = direction*(*vector)[1];
            const int y_movement = direction*(*vector)[0];

            if (pos_x + x_movement <= 7 && 
                pos_y + y_movement <= 7 && 
                pos_x + x_movement >= 0 && 
                pos_y + y_movement >= 0)
            {
                const int square = 8*(pos_x + x_movement) + pos_y + y_movement;

                if (p.colour*GetColour(pos, square) == 1)
                {
                    continue;
                }

                p.moves.push_back(square);
            }
        }
    }
}

void Board::GetPawnMoves(Piece& p, Position& pos, std::vector<int>* legal_moves)
{
    const int parity = (1 - pos.turn)/2;
    Piece& king = pos.pieces[4 + 16*parity];

    if (legal_moves)
    {
        const int pos_y = p.position % 8;
        const int pos_x = (p.position - pos_y)/8;

        if (pos_y + 2*p.colour <= 7 &&
            pos_y + 2*p.colour >= 0 && 
            p.position == 8*p.id - 71 - 123*parity)
        {
            for (auto legal = legal_moves->begin(); legal < legal_moves->end(); ++legal)
            {
                const int square = 8*pos_x + pos_y + 2*p.colour;

                if (square == *legal)
                {
                    p.moves.push_back(square);
                }
            }
        }

        else if (pos_y + p.colour <= 7 &&
                 pos_y + p.colour >= 0)
        {
            for (auto legal = legal_moves->begin(); legal < legal_moves->end(); ++legal)
            {
                const int square = 8*pos_x + pos_y + p.colour;

                if (square == *legal)
                {
                    p.moves.push_back(square);
                }
            }
        }

        if (pos_y + p.colour <= 7 &&
            pos_y + p.colour >= 0 &&
            pos_x + 1 <= 7 &&
            (8*(pos_x + 1) + pos_y + p.colour == king.check_vectors[0] ||
            (8*(pos_x + 1) + pos_y + p.colour == pos.passant && king.check_vectors[0] == pos.passant - p.colour)))
        {
            p.moves.push_back(8*(pos_x + 1) + pos_y + p.colour);
        }

        if (pos_y + p.colour <= 7 &&
            pos_y + p.colour >= 0 &&
            pos_x - 1 <= 7 &&
            (8*(pos_x - 1) + pos_y + p.colour == king.check_vectors[0] ||
            (8*(pos_x - 1) + pos_y + p.colour == pos.passant && king.check_vectors[0] == pos.passant - p.colour)))
        {
            p.moves.push_back(8*(pos_x - 1) + pos_y + p.colour);
        }

        return;
    }

    if (p.pin_vector >= 0)
    {
        if ((p.pin_vector == p.position + 8 + p.colour || p.pin_vector == p.position - 8 + p.colour) &&
             pos.occupancy[p.pin_vector])
        {
            p.moves.push_back(p.pin_vector);
        }

        return;
    }

    const int y = p.position % 8;
    const int x = (p.position - y)/8;

    if (y + 2*p.colour <= 7 &&
        y + 2*p.colour >= 0 && 
        pos.occupancy[8*x + y + p.colour] == 0  &&
        pos.occupancy[8*x + y + 2*p.colour] == 0 &&
        p.position == 8*p.id - 71 - 123*parity)
    {
        p.moves.push_back(8*x + y + p.colour);
        p.moves.push_back(8*x + y + 2*p.colour);
    }

    else if (y + p.colour <= 7 &&
             y + p.colour >= 0 && 
             pos.occupancy[8*x + y + p.colour] == 0)
    {
        p.moves.push_back(8*x + y + p.colour);
    }

    if (y + p.colour <= 7 &&
        y + p.colour >= 0 &&
        x + 1 <= 7 &&
        (p.colour*GetColour(pos, 8*(x + 1) + y + p.colour) == -1 || 8*(x + 1) + y + p.colour == pos.passant))
    {
        p.moves.push_back(8*(x + 1) + y + p.colour);
    }

    if (y + p.colour <= 7 &&
        y + p.colour >= 0 &&
        x - 1 >= 0 &&
        (p.colour*GetColour(pos, 8*(x - 1) + y + p.colour) == -1 || 8*(x - 1) + y + p.colour == pos.passant))
    {
        p.moves.push_back(8*(x - 1) + y + p.colour);
    }
}

void Board::GetKingMoves(Piece& p, Position& pos)
{
    const int parity = (1 - pos.turn)/2;
    const int pos_y = p.position % 8;
    const int pos_x = (p.position - pos_y)/8;
    size_t checks = p.check_vectors.size();
    const std::vector<std::vector<int>>& vectors = King::vectors;

    for (auto vector = vectors.begin(); vector < vectors.end(); ++vector)
    {
        for (int direction = -1; direction < 2; direction += 2)
        {
            const int x_movement = direction*(*vector)[1];
            const int y_movement = direction*(*vector)[0];

            if (pos_x + x_movement <= 7 && 
                pos_y + y_movement <= 7 && 
                pos_x + x_movement >= 0 && 
                pos_y + y_movement >= 0)
            {
                const int square = 8*(pos_x + x_movement) + pos_y + y_movement;
                size_t controlled = pos.control[1 - parity][square].size();

                if (p.colour*GetColour(pos, square) == 1 || controlled)
                {
                    continue;
                }

                p.moves.push_back(square);
            }
        }
    }

    if (checks > 0)
    {
        return;
    }

    if (pos.kingsidecastling[parity] &&
        !pos.occupancy[8*(pos_x + 1) + pos_y] && 
        !pos.occupancy[8*(pos_x + 2) + pos_y] &&
        !pos.control[1 - parity][8*(pos_x + 1) + pos_y].size() &&
        !pos.control[1 - parity][8*(pos_x + 2) + pos_y].size())
    {
        p.moves.push_back(-1);
    }

    if (pos.queensidecastling[(1 - p.colour)/2] && 
        !pos.occupancy[8*(pos_x - 1) + pos_y] && 
        !pos.occupancy[8*(pos_x - 2) + pos_y] && 
        !pos.occupancy[8*(pos_x - 3) + pos_y] &&
        !pos.control[1 - parity][8*(pos_x -1) + pos_y].size() &&
        !pos.control[1 - parity][8*(pos_x -1) + pos_y].size())
    {
        p.moves.push_back(-2);
    }
}

void Board::PrintPosition(Position& pos)
{
    std::cout << std::endl;

    for (int i = 7; i >= 0; --i)
    {
        std::cout << std::endl;

        for (int j = 0; j < 8; ++j)
        {
            if (!pos.occupancy[8*j + i])
            {
                std::cout << " . ";
            }

            else 
            {
                Piece& p = pos.pieces[pos.occupancy[8*j + i] - 1];
                char alignment = p.colour == 1 ? 'w' : 'b';
                std::cout << alignment << p.name << ' ';
            }
        }      
    }
}

void Board::DisplayMoves(const Position& pos)
{
    const int parity = (1 - pos.turn)/2;
    GetMoves(current);

    std::cout << std::endl;

    for (int i = 16*parity; i < 16*(parity + 1); ++i)
    {
        const Piece& p = pos.pieces[i];

        if (p.awake)
        {   
            std::cout << std::endl;

            size_t n_moves = p.moves.size();

            if (!n_moves)
            {
                std::cout << "NULL";
                continue;
            }

            for (auto move = p.moves.begin(); move < p.moves.end(); ++move)
            {
                if (*move == -1)
                {
                    std::cout << "O-O" << ' ';
                    continue;
                }

                else if (*move == -2)
                {
                    std::cout << "OOO" << ' ';
                    continue;
                }


                std::cout << p.name << GetSquare(*move) << ' ';
            }
        }
    }
}

Board::Board()
{
    current = Position();
}

Board::~Board()
{}