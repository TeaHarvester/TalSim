#include "Helpers.h"

std::string GetSquare(int index)
{
    int rank = index % 8;
    int file = (index - rank)/8;
    char output[2] = {char(file + 97), char(rank + 49)};

    return std::string(output);
}

size_t HashPolicy::operator()(const Position& pos) const
    {
        std::string hash(54, '\0');

        for (int i = 0; i < 32; ++i)
        {
            hash[i] = pos.pieces[i].position;
        }

        hash[32] = pos.kingsidecastling[0];
        hash[33] = pos.kingsidecastling[1];
        hash[34] = pos.queensidecastling[0];
        hash[35] = pos.queensidecastling[1];
        hash[36] = pos.turn;

        for (int i = 0; i < 8; ++i)
        {
            hash[36 + i] = pos.pieces[i + 8].name;
            hash[44 + i] = pos.pieces[i + 24].name;
        }

        std::hash<std::string> output;
        return (output(hash));
    }