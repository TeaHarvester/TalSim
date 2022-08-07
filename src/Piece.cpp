#include "Piece.h"

bool Piece::Legal(int destination)
{
    int n_moves = moves.size();

    for (int i = 0; i < n_moves; ++i)
    {
        if (moves[i] == destination)
        {
            return true;
        }
    }

    std::cout << std::endl << "can't move there!" << std::endl;
    return false;
}

Piece::Piece()
{}

Piece::Piece(int number, int pos) :
id(number),
colour(id <= 16? 1 : -1),
position(pos),
awake(true),
pin_vector(-1)
{}

Pawn::Pawn(int number, int pos) : 
Piece(number, pos)
{
    name = 'p';
}

Knight::Knight(int number, int pos) : 
Piece(number, pos)
{
    name = 'n';
}

Bishop::Bishop(int number, int pos) : 
Piece(number, pos)
{
    name = 'b';
}

Rook::Rook(int number, int pos) : 
Piece(number, pos)
{
    name = 'r';
}

Queen::Queen(int number, int pos) : 
Piece(number, pos)
{
    name = 'q';
}

King::King(int number, int pos) : 
Piece(number, pos)
{   
    name = 'k';
}

const std::vector<std::vector<int>> Queen::vectors = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
const std::vector<std::vector<int>> Knight::vectors = {{1, -2}, {2, -1}, {2, 1}, {1, 2}};
const std::vector<std::vector<int>> Bishop::vectors = {{1, 1}, {1, -1}};
const std::vector<std::vector<int>> Rook::vectors = {{1, 0}, {0, 1}};
const std::vector<std::vector<int>> Pawn::vectors = {{2, 0}, {1, 0}, {1, 1}, {1, -1}};
const std::vector<std::vector<int>> King::vectors = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};