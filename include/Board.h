#ifndef BOARD
#define BOARD

#include<iostream>
#include<vector>
#include<string>
#include "piece.h"
#include "Position.h"

class Board
{
    public:
    std::vector<Piece*> pieces;
    Position* current;

    void PrintPosition(Position*& pos);
    void GetMoves(Position*& pos);
    void DisplayMoves(Position*& pos);
    void Move(std::string move, char prom = 'q');
    void Move(int id, int destination, char prom = 'q');
    Position* Move(int id, int destination, Position*& pos, char prom = 'q');
    Board();
    ~Board();

    private:
    void Arrange(Position*& pos);
    void GetMoves(Piece*& p, Position*& pos);
    void TrimMoves(Position*& pos);
    bool Check(Position*& pos);
};

#endif