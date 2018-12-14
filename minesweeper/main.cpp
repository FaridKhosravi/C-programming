// project 1: 3.3.3 (P) Minesweeper
// Farid Khosravi - farid.khosravi@student.tut.fi - 267964
// I have not changed anything in this code.

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "square.hh"

const int SIZE = 6;

// Function prints the board in certain format.
void printBoard(std::vector< std::vector< Square > >& board, std::ostream& stream)
{
    // Printing space after each character to make ASCII graphics clearer

    // Printing numbers of x-axis over the board
    stream << "  ";
    for(int x = 0; x < SIZE; ++x)
    {
        stream << (x+1) % 10 << ' ';  // Using modulo 10 to make the output fit, even if the board width was more than 10
    }
    stream << std::endl;

    // Printing the board such that each line begins with y-coordinate, and then each
    // square of the line is asked to print itself
    for(int y = 0; y < SIZE; ++y)
    {
        stream << (y+1) % 10 << ' ';
        for(int x = 0; x < SIZE; ++x)
        {
            board.at(y).at(x).print(stream);
            stream << " ";
        }
        stream << std::endl;
    }
}

// Function initializes the board, i.e. forms square objects and calculates data needed by them
void initBoard(std::vector< std::vector< Square > >& board)
{
    std::default_random_engine randomEng;

    std::cout << "Give a seed value or an empty line: ";
    std::string seed = "";
    getline(std::cin, seed);

    if(seed == "") {
        // If the user did not give a seed value, use computer time as the seed value.
        randomEng.seed(time(NULL));
    } else {
        // If the user gave a seed value, use it.
        randomEng.seed(stoi(seed));
    }

    std::uniform_int_distribution<int> distribution(0, 4);

    for(int y = 0; y < SIZE; ++y)
    {
        std::vector< Square > line;
        for(int x = 0; x < SIZE; ++x)
        {
            bool hasMine = false;
            if(distribution(randomEng) == 0){
                hasMine = true;
            }
            Square new_square(x, y, hasMine, &board);
            line.push_back(new_square);
        }
        board.push_back(line);
    }

    // When all the squares of the board exist, each square is asked to calculate
    // the data telling how many mines lie in the adjacent squares.
    // After that game board is ready.
    for(int y = 0; y < SIZE; ++y)
    {
        for(int x = 0; x < SIZE; ++x)
        {
            board.at(y).at(x).countAdjacent();
        }
    }
}

// Function checks if the game is over
bool isGameOver(std::vector< std::vector< Square > >& board)
{
    for(int y = 0; y < SIZE; ++y)
    {
        for(int x = 0; x < SIZE; ++x)
        {
            if(not board[x][y].isReady())
            {
                return false;
            }
        }
    }
    return true;
}

// Function prints the question given as a parameter and interpretes the given string
bool flag_question(std::string question){
    std::string answer = "";
    do
    {
        std::cout << question;
        std::cin >> answer;
    } while(answer != "Y" and answer != "y" and answer != "N" and answer != "n");

    return (answer == "Y" or answer == "y");
}

// Function reads values for x and y that passed by reference for the caller
bool readCoordinates(int& x, int& y)
{
    std::cout << "x y> ";
    return std::cin >> x and std::cin >> y;
}

int main()
{
    std::vector< std::vector< Square > > board;
    initBoard(board);
    printBoard(board, std::cout);

    int x = 0;
    int y = 0;

    while(not isGameOver(board) and readCoordinates(x, y))
    {
        if(board.at(y-1).at(x-1).hasFlag())
        {
            if(flag_question("Do you want to remove the flag? (Y/N) "))
            {
                board.at(y-1).at(x-1).removeFlag();
            }
        } else if(not flag_question("Do you want to add a flag? (Y/N) ")) {
            if(not board.at(y-1).at(x-1).open()) {
                std::cout << "BOOOM! Game over!" << std::endl;
                printBoard(board, std::cout);
                std::cout << "You lost ..." << std::endl;
                return EXIT_FAILURE;
            }
        } else {
            board.at(y-1).at(x-1).addFlag();
        }
        printBoard(board, std::cout);
    }
    std::cout << "You won, yay!" << std::endl;
    return EXIT_SUCCESS;
}
