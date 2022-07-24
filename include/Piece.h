#ifndef PIECE
#define PIECE

#include<iostream>
#include<vector>
#include<array>

struct Piece
{   
    int id;
    int colour;
    int position;
    char name;
    bool moved;
    std::vector<int> moves;
    std::vector<std::vector<int>> vectors;
    std::vector<std::vector<int>> hops;

    bool Legal(int destination);
    Piece();
    Piece(int number, int pos);
};

struct Pawn : public Piece
{
    public:
    bool moved;
    
    Pawn(int number, int pos);
};

struct Knight : public Piece
{
    public:
    Knight(int number, int pos);
};

struct  Bishop : public Piece
{
    public:
    Bishop(int number, int pos);
};

struct Rook : public Piece
{
    public:
    bool moved;

    Rook(int number, int pos);
};

struct Queen : public Piece
{
    public:
    Queen(int number, int pos);
};

struct King : public Piece
{
    public:
    bool moved;

    King(int number, int pos);
};

#endif