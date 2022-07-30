#ifndef POSITION
#define POSITION

#include<iostream>
#include<vector>
#include "Piece.h"

struct Position
{
    int turn;
    int occupancy[64];
    // int piece_occupancy[32];
    int passant;
    // bool moved[16];
    bool kingsidecastling[2];
    bool queensidecastling[2];
    // char promotions[16];
    float evaluation;
    Piece* pieces[32];

    void GetMoves();
    void GetMoves(const int id);
    void TrimMoves();
    int GetColour(const int square);
    int Move(const int id, const int destination, const char prom = 'q');
    Position();
    Position(const Position* pos);
    // Position(const Position* pos, int id, int destination, char prom = 'q');
};

#endif