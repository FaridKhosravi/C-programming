/*
#####################################################################
# TIE-02207 Programming 2: Basics, F2018                            #
# Project4: Slots                                                   #
# Program description: Implements a virtual slots game.             #
# File: reel.cpp                                                    #
# Description: Draws random fruits for labels to display.           #
# Notes: * This is an exercise program.                             #
#        * Students need to modify this file.                       #
#####################################################################
*/
// This is the fourth Project of Programming 2: Basics TIE-02207
// Farid Khosravi - Student Number: 267964 - Email: farid.khosravi@student.tut.fi
// this file contains the methods of the class Reel

#include "reel.hh"
#include <vector>
#include <string>
#include <algorithm>

// Constructor of the class is implemented exactly in a way that asks in the comments of the raw file
Reel::Reel(const std::vector<QLabel*>& labels,
           QPushButton* lock_button,
           const Fruits* fruits,
           std::shared_ptr<std::default_random_engine> rng) :
        labels_(labels), lock_button_(lock_button), fruits_(fruits), rng_(rng){

    for ( auto fruit_data: *fruits_ ) {
        known_fruit_names_.push_back(fruit_data.first);
    }

    lock_button_->setCheckable(true);
    lock_button_->setChecked(false);

    spinning_ = false;

    random_select_and_set_fruits();

    connect(&timer_, &QTimer::timeout, this, &Reel::spin_once);
}

// This method checks if the lock button of the reel is not clicked then spins the reel
void Reel::init_spin() {
    if ( not lock_button_-> isChecked()) {

        // Number of the spins is selected randomly between 10 to 30
        how_many_spins_ = randint(10, 30);

        // when the reel spins spinning_ is true
        spinning_ = true;

        // the time of spinning is 50ms
        timer_.start(50);
    }

}

// returns the spinning_
bool Reel::is_spinning() const {
    return spinning_;
}

// returns the labels of each reels by receiving the number of the reel as index
std::string Reel::get_symbol(unsigned index) const {
    return shown_fruit_names_[index];
}

// selects and sets fruits on labels in each spin
void Reel::spin_once() {

    // this method selects and sets the fruits
    random_select_and_set_fruits();

    --how_many_spins_;

    // if the number of spinning finieshed stooping the timer and emmiting the middle row of the reels
    if (how_many_spins_ == 0){
        timer_.stop();
        spinning_ = false;
        emit stopped(shown_fruit_names_[1]);
    }
}

// distributing a random integere between a min and max integer
int Reel::randint(int min, int max) const {
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(*rng_);
}

// generate random integers and collect them in vector and return the vector
std::vector<int> Reel::get_unique_random_ints(unsigned how_many, unsigned min, unsigned max) const {
    std::vector<int> numbers = { };
    while (numbers.size() < how_many ) {
        int num = randint(min, max);
        if ( std::find(numbers.begin(), numbers.end(), num) == numbers.end() ){
            numbers.push_back(num);
        }
    }
    return numbers;
}

// this method selects and sets the fruits
void Reel::random_select_and_set_fruits() {

    shown_fruit_names_.clear();

    // generate the vector of random integers and push them in the shown_fruit_names_
    std::vector<int> indexes = get_unique_random_ints(3, 0, known_fruit_names_.size() - 1);
    for( int index:indexes ) {
        shown_fruit_names_.push_back(known_fruit_names_[index]);
    }

    // showing fruits name on each reel labels
    for (int index: {0, 1, 2}) {
        std::string fruit_name = shown_fruit_names_[index];
        labels_.at(index)->setPixmap( fruits_->at(fruit_name).first );
    }
}


