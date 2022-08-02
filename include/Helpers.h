#ifndef HELPERS
#define HELPERS

#include <iostream>
#include <string>
#include <Position.h>

std::string GetSquare(int index);

struct HashPolicy
{
    size_t operator()(const Position& pos) const
    {
        std::string hash(54, '\0');

        for (int i = 0; i < 16; ++i)
        {
            hash[i] = pos.piece_occupancy[i];
            hash[i + 16] = pos.piece_occupancy[i + 16];
        }

        hash[32] = pos.kingsidecastling[0];
        hash[33] = pos.kingsidecastling[1];
        hash[34] = pos.queensidecastling[0];
        hash[35] = pos.queensidecastling[1];
        hash[36] = pos.turn;

        std::hash<std::string> output;
        return (output(hash));
    }
};

#endif