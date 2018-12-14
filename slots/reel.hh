/*
#####################################################################
# TIE-02207 Programming 2: Basics, F2018                            #
# Project4: Slots                                                   #
# Program description: Implements a virtual slots game.             #
# File: reel.h                                                      #
# Description: Draws random fruits for labels to display.           #
# Notes: * This is an exercise program.                             #
#        * Students need to modify this file.                       #
#####################################################################
*/
// This is the fourth Project of Programming 2: Basics TIE-02207
// Farid Khosravi - Student Number: 267964 - Email: farid.khosravi@student.tut.fi
// The Reel class contains 9 labels which that each three of them are in a reel.
// Each reel also has a lock button to prevent reel to change the pictures in each label.
// This file is the header of the Reel class shich contains the methods, signal, private
// parameters and slots of this class.



#ifndef SLOTS_REEL_H
#define SLOTS_REEL_H

#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <QPushButton>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include <QTimer>

// Fruits is an alias for a map which contains a fruit's name and
// corresponding image and likelyhood of appearance.
using Fruits = std::map<std::string, std::pair<QPixmap, int>>;


// class Reel determines if the player has won.
// Reel spins for a random duration and draws random fruits to display on its
// labels.
//
class Reel: public QObject {
    Q_OBJECT

    public:

    // Construct of a new Reel.
    // labels are used to display the symbols.
    // lock_button Button whose lock each Reel.
    // fruits Images and frequencies of each fruit.
    // rng Random number generator used to determine the symbols.

    Reel(const std::vector<QLabel*>& labels,
         QPushButton* lock_button,
         const Fruits* fruits,
         std::shared_ptr<std::default_random_engine> rng);

    // this method checks if the reel is not locked spin it.
    void init_spin();

    // this method returns spinning_ private variable
    bool is_spinning() const;

    // this method returns the name of the fruit which is in the each label of the reels
    std::string get_symbol(unsigned index) const;

    signals:
    // this signal emmits the middle symbols of each reel but I have not used it to detect
    // the similarity in fruits in the rows or diagonals.
        void stopped(const std::string& middle_sym);

    private slots:
        // each cycle of spinning is done by this method
        void spin_once();

    private:
        // this method generates random integer
        int randint(int min, int max) const;

        // this method collects the random integers in a vector and return the vector
        std::vector<int> get_unique_random_ints(unsigned how_many, unsigned min, unsigned max) const;

        // this function select and sets athe fruits in each reel
        void random_select_and_set_fruits();

        // the rest of the private variables are clear by their name
        std::vector<QLabel*> labels_;
        QPushButton* lock_button_;
        const Fruits* fruits_;
        std::shared_ptr<std::default_random_engine> rng_;

        // contains name of the fruits
        std::vector<std::string> known_fruit_names_;

        // contains name of the fruits in the labels of each reel
        std::vector<std::string> shown_fruit_names_;

        // when reels are spinning it is true otherwise it is false
        bool spinning_;
        QTimer timer_;

        // it is the number of spinning
        int how_many_spins_;

};  // class Reel


#endif  // SLOTS_REEL_H
