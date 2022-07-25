#include <iostream>
#include "Board.h"
#include "Piece.h"

int main()
{
    Board b;
    b.Move(12, 27);
    b.Move(27, 20);
    b.Move(12, 20);
    b.Move(26, 12);
    b.Move(12, 13);
    b.Move(25, 5);
    b.Move(12, 14);
    b.Move(25, 4);
    b.Move(12, 7, 'n');
    b.Move(25, 3);
    b.PrintPosition(b.current);
    b.DisplayMoves(b.current);

    return 0;
}