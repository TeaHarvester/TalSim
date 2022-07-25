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
position(pos)
{
    vectors = std::vector<std::vector<int>>(1);
    vectors[0] = std::vector<int>{0, 0};
}

Pawn::Pawn(int number, int pos) : 
Piece(number, pos)
{
    name = 'p';
}

Knight::Knight(int number, int pos) : 
Piece(number, pos)
{
    name = 'n';

    hops = std::vector<std::vector<int>>(8);
    hops[0] = std::vector<int>{2, 1};
    hops[1] = std::vector<int>{1, 2};
    hops[2] = std::vector<int>{-2, 1};
    hops[3] = std::vector<int>{-1, 2};
    hops[4] = std::vector<int>{2, -1};
    hops[5] = std::vector<int>{1, -2};
    hops[6] = std::vector<int>{-2, -1};
    hops[7] = std::vector<int>{-1, -2};
}

Bishop::Bishop(int number, int pos) : 
Piece(number, pos)
{
    name = 'b';

    vectors = std::vector<std::vector<int>>(2);
    vectors[0] = std::vector<int>{1, 1};
    vectors[1] = std::vector<int>{1, -1};
}

Rook::Rook(int number, int pos) : 
Piece(number, pos)
{
    name = 'r';

    vectors = std::vector<std::vector<int>>(2);
    vectors[0] = std::vector<int>{1, 0};
    vectors[1] = std::vector<int>{0, 1};
}

Queen::Queen(int number, int pos) : 
Piece(number, pos)
{
    name = 'q';

    vectors = std::vector<std::vector<int>>(4);
    vectors[0] = std::vector<int>{1, 0};
    vectors[1] = std::vector<int>{0, 1};
    vectors[2] = std::vector<int>{1, 1};
    vectors[3] = std::vector<int>{-1, 1};
}

King::King(int number, int pos) : 
Piece(number, pos)
{   
    name = 'k';

    hops = std::vector<std::vector<int>>(8);
    hops[0] = std::vector<int>{1, 0};
    hops[1] = std::vector<int>{0, 1};
    hops[2] = std::vector<int>{1, 1};
    hops[3] = std::vector<int>{-1, 0};
    hops[4] = std::vector<int>{0, -1};
    hops[5] = std::vector<int>{-1, 1};
    hops[6] = std::vector<int>{1, -1};
    hops[7] = std::vector<int>{-1, -1};
}

