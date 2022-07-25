#ifndef POSITION
#define POSITION

#include<iostream>

struct Position
{
    int turn;
    int occupancy[64];
    int piece_occupancy[32];
    int passant;
    bool moved[16];
    bool kingsidecastling[2];
    bool queensidecastling[2];
    char promotions[16];
    float evaluation;

    int GetColour(int square);
    Position();
    Position(const Position* pos);
    Position(const Position* pos, int id, int destination, char prom = 'q');
};

#endif