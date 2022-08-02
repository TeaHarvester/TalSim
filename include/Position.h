#ifndef POSITION
#define POSITION

#include <iostream>
#include <vector>
#include <utility>

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
    std::vector<std::pair<int, int>> movelist;

    Position();
    Position(const Position* pos);
    Position(const Position* pos, int id, int destination, char prom = 'q');

    bool operator==(const Position& pos) const;
};

#endif