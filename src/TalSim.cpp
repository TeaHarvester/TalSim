#include <iostream>
#include "Board.h"
#include "Piece.h"

int main()
{
    Board b;
    b.PrintPosition(b.current);
    // b.Move(9, 3);
    // b.Move(25, 4);
    // b.Move(7, 42);
    // b.Move(29, 36);
    b.Move(9, 3);
    b.Move(25, 4);
    b.Move(10, 11);
    b.Move(25, 11);
    b.Move(11, 19);
    b.Move(25, 18);
    b.PrintPosition(b.current);
    b.DisplayMoves(b.current);

    return 0;
}