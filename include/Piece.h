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
    std::vector<int> moves;
    std::vector<std::vector<int>> vectors;
    std::vector<std::vector<int>> hops;

    bool Legal(int destination);
    Piece();
    Piece(int number, int pos);
};

struct Pawn : public Piece
{
    Pawn(int number, int pos);
};

struct Knight : public Piece
{
    Knight(int number, int pos);
};

struct  Bishop : public Piece
{
    Bishop(int number, int pos);
};

struct Rook : public Piece
{
    Rook(int number, int pos);
};

struct Queen : public Piece
{
    Queen(int number, int pos);
};

struct King : public Piece
{
    King(int number, int pos);
};

#endif