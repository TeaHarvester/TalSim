#ifndef HELPERS
#define HELPERS

#include <iostream>
#include <string>
#include <Position.h>

std::string GetSquare(int index);

struct HashPolicy
{
    size_t operator()(const Position& pos) const;
};

// const int queenvectors[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
// const int knightvectors[4][2] = {{1, 2}, {2, 1}, {1, -2}, {2, -1}};
// const int bishopvectors[2][2] = {{1, 1}, {1, -1}};
// const int rookvectors[2][2] = {{1, 0}, {0, 1}};
// const int pawnvectors[4][2] = {{2, 0}, {1, 0}, {1, 1}, {1, -1}};

#endif