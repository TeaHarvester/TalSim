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
    bool awake;
    char name;
    std::vector<int> moves;
    std::vector<int> check_vectors;
    int pin_vector;
    static const std::vector<std::vector<int>> vectors;

    bool Legal(int destination);
    Piece();
    Piece(int number, int pos);
};

struct Pawn : public Piece
{
    Pawn(int number, int pos);
    static const std::vector<std::vector<int>> vectors;
};

struct Knight : public Piece
{
    Knight(int number, int pos);
    static const std::vector<std::vector<int>> vectors;
};

struct  Bishop : public Piece
{
    Bishop(int number, int pos);
    static const std::vector<std::vector<int>> vectors;
};

struct Rook : public Piece
{
    Rook(int number, int pos);
    static const std::vector<std::vector<int>> vectors;
};

struct Queen : public Piece
{
    Queen(int number, int pos);
    static const std::vector<std::vector<int>> vectors;
};

struct King : public Piece
{
    King(int number, int pos);
    static const std::vector<std::vector<int>> vectors;
};


#endif