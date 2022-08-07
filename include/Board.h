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
    // std::vector<Piece> pieces;
    Position current;

    void PrintPosition(Position& pos);
    void GetMoves(Position& pos);
    void DisplayMoves(const Position& pos);
    void Move(const std::string move, const char prom = 'q');
    void Move(const int id, const int destination, const char prom = 'q');
    void Move(const int id, const int destination, Position& pos, const char prom = 'q');
    Board();
    ~Board();

    private:
    // void Arrange(Position& pos);
    void GetVectorControl(Piece& p, Position& pos);
    void GetHopControl(Piece& p, Position& pos);
    void GetPawnControl(Piece& p, Position& pos);

    void GetVectorMoves(Piece& p, Position& pos, std::vector<int>* legal_moves = NULL);
    void GetKnightMoves(Piece& p, Position& pos, std::vector<int>* legal_moves = NULL);
    void GetPawnMoves(Piece& p, Position& pos, std::vector<int>* legal_moves = NULL);
    void GetKingMoves(Piece& p, Position& pos);
    void GetPinVectors(Position& pos);
    // void TrimMoves(Position& pos);
    // bool Check(Position& pos, const int colour);
    int GetColour(Position& pos, const int square);
    // int GetCheck(Position& pos, const int square);
    // char GetAggressor(Position& pos,  const int square, const int colour);
};

#endif