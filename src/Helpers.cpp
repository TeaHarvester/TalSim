#include "Helpers.h"

std::string GetSquare(int index)
{
    int rank = index % 8;
    int file = (index - rank)/8;
    std::string output = {char(file + 97), char(rank + 49)};

    return output;
}