#ifndef BOARD
#define BOARD

#include<iostream>
#include<vector>
#include "piece.h"
#include "Position.h"

class Board
{
    public:
    std::vector<Piece*> pieces;
    Position* current;

    void PrintPosition(Position*& pos);
    void DisplayMoves(Position*& pos);
    void Move(int id, int destination);
    Position* Move(int id, int destination, Position*& pos);
    Board();
    Board(const Board& b);
    ~Board();

    private:
    void Arrange(Position*& pos);
    void GetMoves(Position*& pos);
    void GetMoves(Piece*& p, Position*& pos);
    bool Check(int colouor, Position*& pos);
};

#endif