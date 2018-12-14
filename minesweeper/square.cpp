// project 1: 3.3.3 (P) Minesweeper
// Farid Khosravi - farid.khosravi@student.tut.fi - 267964
// This file contains implementation of each method of the class Square. I describe each method before its implementation.

#include "square.hh"
#include <vector>
#include <iostream>


// In this method each object of the class initializes and we assigned below private variables to the object
// inputs of this method is coordinations of the square which are integer and the has_mine information of the square which is boolean
// and the pointer vector type board.
// the output is object of Square class.
Square::Square(int x, int y, bool has_mine, std::vector<std::vector<Square>>* board) {
    x_ = x;
    y_ = y;
    has_mine_ = has_mine;
    board_ = board;
}

// In this method we find the number of mines around each square and assigne it to the mines_around_ private variable of the square.
// there is no input and output for this method.
void Square::countAdjacent() {
    int size = board_->size();
    for (int x = x_ - 1; x <= x_ + 1; ++x) {
        for (int y = y_ - 1; y <= y_ + 1; ++y) {
            if (x < 0 or y < 0 or x >= size or y >= size) {
                continue;
            }
            if (x == x_ and y == y_) {
                continue;
            }
            if (board_->at(y).at(x).has_mine_) {
                ++mines_around_;
            }
        }
    }
}

// This method prints each square based on its pravate information such as it is flagged or not or is it opened or not.
// the input of this method is a stream object which can writes sequences of characters and represent other kinds of data.
// and the output is obviously printing on display.
void Square::print(std::ostream& stream) const {
    if (not flagged_ and not opened_) {
        stream << ".";
    } else if (flagged_) {
        stream << "P";
    } else if (opened_ and not has_mine_) {
        stream << mines_around_;
    } else if (opened_ and has_mine_){
        stream << "*";
    }
}

// This method reveals that is there any square with mine and not flagged or all of them are opened and all
// the remained squares do not have mine.
// There is no input for this method and the output is boolean.
bool Square::isReady() const {
    if ((flagged_ and has_mine_) or (not flagged_ and not has_mine_) or opened_) {
        return true;
    } else {
        return false;
    }
}

// This method returnes the flagged_ privater variable of each square object.
// there is no input and the output is boolean value of the flagged_ variable.
bool Square::hasFlag() const {
    return flagged_;
}

// In this method the value of the flagged_ Private variable will changed to false.
// No input and no output.
void Square::removeFlag() {
    flagged_ = false;
}

// In this method the value of the flagged_ Private variable will changed to true.
// No input and no output.
void Square::addFlag() {
    flagged_ = true;
}

// The most challenging method of this class is this method.
// In this method the squares will be opened in a recursive method.
// There is no input for this method and the output is boolean.
// this method reads the private data of each square and if the square has mine and is not flagged
// retunes false. In other cases when the mines_around_ variable has any value else than
// zero just open the square. If the mines_around_ is equal to zero the recursive process will start by
// calling itself and this process will continue until there are no square with mine_around_ equal to zero
// in its around.
bool Square::open() {
    if (has_mine_ and not flagged_) {
        board_->at(y_).at(x_).opened_ = true;
        return false;
    } else if (not has_mine_ and not flagged_ and mines_around_ != 0) {
        board_->at(y_).at(x_).opened_ = true;
    } else if (not has_mine_ and not flagged_ and mines_around_ == 0){
        for (int x = x_ - 1; x <= x_ + 1; ++x) {
            for (int y = y_ - 1; y <= y_ + 1; ++y) {
                int size = board_->size();
                if (x < 0 or y < 0 or x >= size or y >= size) {
                    continue;
                } else if (not has_mine_ and not flagged_) {
                    if (board_->at(y).at(x).mines_around_ == 0 and not board_->at(y).at(x).opened_) {
                        board_->at(y).at(x).opened_ = true;
                        board_->at(y).at(x).open();
                    }
                    board_->at(y).at(x).opened_ = true;
                }

            }
        }
    }
    return true;
}
