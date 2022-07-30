#ifndef PIECE
#define PIECE

#include<iostream>
#include<vector>
#include<array>

struct Piece
{   
    int id;
    int position;
    char name;
    std::vector<int> moves;
    std::vector<std::vector<int>> vectors;
    std::vector<std::vector<int>> hops;

    bool Legal(int destination);
    virtual Piece* Copy() = 0;
    Piece();
    Piece(int number, int pos);

    virtual ~Piece(){};
};

struct Pawn : public Piece
{   
    Piece* Copy();
    Pawn(int number, int pos);
    ~Pawn(){};
};

struct Knight : public Piece
{   
    Piece* Copy();
    Knight(int number, int pos);
    ~Knight(){};
};

struct  Bishop : public Piece
{
    Piece* Copy();
    Bishop(int number, int pos);
    ~Bishop(){};
};

struct Rook : public Piece
{
    Piece* Copy();
    Rook(int number, int pos);
    ~Rook(){};
};

struct Queen : public Piece
{
    Piece* Copy();
    Queen(int number, int pos);
    ~Queen(){};
};

struct King : public Piece
{
    Piece* Copy();
    King(int number, int pos);
    ~King(){};
};

#endif