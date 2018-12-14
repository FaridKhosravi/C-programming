// project 1: 3.3.3 (P) Minesweeper
// Farid Khosravi - farid.khosravi@student.tut.fi - 267964
//
// This file is the interface of the class Square and in the Public part I defined the methods
// which are used in the main.cpp file and in the Private part I defined the private variables
// of each object of this class. The class Square is defined to make some square objects with
// specific features for our minesweeper game.
// In the Private part we have 7 feature for each object: int x_, int y_ show the coordination of the object,
// bool has_mine_ is a boolean variable which shows the square has mine inside or not.
// vector<vector<Square>>* board_ is a pointer with type of vector to give us ability of pointing to our specific square.
// bool Opened_ is a boolean variable which expresses the square is opened or not.
// bool flagged_ is a boolean variable which shows this square is flagged or not.
// int mines_around_ is an integer variable that shows the number of mines around each square.
// In the Public part we have 8 methods which I describe them before each method in the Square.cpp file.

#ifndef SQUARE_HH
#define SQUARE_HH
#include <vector>
#include <iostream>


class Square {
    public:
        Square(int x, int y, bool has_mine, std::vector<std::vector<Square>>* board);
        void countAdjacent();
        void print(std::ostream& stream) const;
        bool isReady() const;
        bool hasFlag() const;
        void removeFlag();
        void addFlag();
        bool open();

    private:
        int x_;
        int y_;
        bool has_mine_;
        std::vector<std::vector<Square>>* board_;
        bool opened_ = false;
        bool flagged_ = false;
        int mines_around_ = 0;
        
};

#endif // SQUARE_HH
