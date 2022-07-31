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

    bool operator==(const Position& pos);
};

    bool Position::operator==(const Position& pos)
    {
        if (movelist.size() != pos.movelist.size())
        {
            return false;
        }

        auto iter1 = movelist.begin();
        auto iter2 = pos.movelist.begin();

        while (iter1 < movelist.end())
        {
            if (iter1->first != iter2->first || iter1->second != iter2->second)
            {
                return false;
            }
        }

        return true;
    }

#endif