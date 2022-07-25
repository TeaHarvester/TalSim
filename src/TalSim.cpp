#include <iostream>
#include "Board.h"
#include "Piece.h"

int main()
{
    Board b;
    b.Move(12, 27);
    b.talsim.Evaluate(b.current);
    b.Move(27, 20);
    b.talsim.Evaluate(b.current);
    b.Move(12, 20);
    b.talsim.Evaluate(b.current);
    b.Move(26, 12);
    b.talsim.Evaluate(b.current);
    b.Move(12, 13);
    b.talsim.Evaluate(b.current);
    b.Move(25, 5);
    b.talsim.Evaluate(b.current);
    b.Move(12, 14);
    b.talsim.Evaluate(b.current);
    b.Move(25, 4);
    b.talsim.Evaluate(b.current);
    b.Move(12, 7, 'q');
    b.talsim.Evaluate(b.current);
    b.Move(25, 3);
    b.talsim.Evaluate(b.current);
    b.PrintPosition(b.current);
    b.DisplayMoves(b.current);

    return 0;
}