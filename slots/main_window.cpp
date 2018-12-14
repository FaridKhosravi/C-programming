/*
#####################################################################
# TIE-02207 Programming 2: Basics, F2018                            #
# Project4: Slots                                                   #
# Program description: Implements a virtual slots game.             #
# File: main_window.cpp                                             #
# Description: Implements a UI for the game to be playable.         #
# Notes: * This is an exercise program.                             #
#        * Students need to modify this file.                       #
#####################################################################
*/
// This is the fourth Project of Programming 2: Basics TIE-02207
// Farid Khosravi - Student Number: 267964 - Email: farid.khosravi@student.tut.fi
// In this file we have the codes related to the methods of the header file.

// *** When I run the code via TigerVNC in my own laptop, there is no problem in compiling,
// but after that it shows me an error about failing connection of Slots(the name of the program)
// and socket. I do not have this error when I run my code on Kooditorioon PCs. TA Mikko said
// it is not important and we care just about compiling errors.


#include "main_window.hh"
#include "reel.hh"
#include <QPixmap>
#include <QString>
#include <chrono>
#include <map>
#include <memory>
#include <random>
#include <utility>
#include <iostream>
#include <QPalette>


// I put the frequency here to be able to use it in all parts of my code.
const std::map<std::string, int> frequencies{
    {"cherries", 56},
    {"strawberry", 50},
    {"orange", 46},
    {"pear", 39},
    {"apple", 35},
    {"bananas", 30},
    {"tomato", 24},
    {"grapes", 15},
    {"eggplant", 5}
};

namespace {

// brief Get the fruit symbols and their frequencies.
// return A map of image names and corresponding images and frequencies.
Fruits loadFruits() {
    // Set frequencies for each symbol.
    // * You can freely play around with the values below.
    // * Values are relative to each other, higher == more common.
    // * Names of these fruits (and berries too) have to actually match
    //   images found on the disk. The images must be in the PNG format.


    // Define where the images can be found and what kind of images they are.
    // * Here we assume that the build directory is next to Slots directory.
    const std::string directory("../slots/Resources/");
    const std::string extension(".png");

    // Load the images into QPixmaps and bundle them with their frequencies.
    Fruits symbols;
    for (const std::pair<std::string, int>& frequency : frequencies) {
        const std::string filename = directory + frequency.first + extension;
        const QPixmap image(QString::fromStdString(filename));
        symbols[frequency.first] = std::make_pair(image, frequency.second);
    }

    return symbols;
}

}  // namespace

// Constructor of the Class
MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent),
    fruits_(loadFruits()) {

    ui_.setupUi(this);

    // this is the code which I used to change the background color of
    // the MainWindow.
    // In this part my code shows an error "libpng warning: iCCP: known incorrect sRGB profile"
    // that TA Mikko said the TAs also have this problem and it is not important.
    QPixmap background(":/MainWindow/Resources/tile.png");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, background);
    this->setPalette(palette);

    // Initializing the UI
    initUi();
}

// This method initializes the Main Window
void MainWindow::initUi() {

    const auto time = std::chrono::high_resolution_clock::now();
    const auto seed = time.time_since_epoch().count();
    auto rng = std::make_shared<std::default_random_engine>(seed);

    // The labels which are used to show the fruits sumbols
    std::vector<std::vector<QLabel*>> fruitLabels = {
        {ui_.fruitLabel_0_0, ui_.fruitLabel_1_0, ui_.fruitLabel_2_0},
        {ui_.fruitLabel_0_1, ui_.fruitLabel_1_1, ui_.fruitLabel_2_1},
        {ui_.fruitLabel_0_2, ui_.fruitLabel_1_2, ui_.fruitLabel_2_2},
    };

    // a vector contains lock buttons
    std::vector<QPushButton*> lockButtons = {
        ui_.LockSlot_1, ui_.LockSlot_2, ui_.LockSlot_3
    };

    ui_.firstThird->setCheckable(true);ui_.firstThird->setChecked(false);
    ui_.diagonalButton->setCheckable(true);ui_.diagonalButton->setChecked(false);


    ui_.enteryCredit->setText("Enter credit...");
    ui_.spinRemained->setStyleSheet("background-color: white");
    ui_.spinRemained->display(0);

    // Making new object for each reel and push them into reels_ vector and connect them
    // to the reelStopped method
    for ( unsigned int index = 0; index < lockButtons.size(); ++index) {
        Reel* reel = new Reel(fruitLabels[index],
                              lockButtons[index],
                              &fruits_, rng);
                reels_.push_back(reel);
                connect(reel, &Reel::stopped, this, &MainWindow::reelStopped);
    }

    // connect each button to the proper method
    connect(ui_.spinButton, &QPushButton::clicked, this, &MainWindow::spin_clicked);
    connect(ui_.firstThird, &QPushButton::clicked, this, &MainWindow::first_third_rows);
    connect(ui_.diagonalButton, &QPushButton::clicked, this, &MainWindow::diagonal);
    connect(ui_.creditEntery, &QPushButton::clicked, this, &MainWindow::credit_entery_clicked);
    connect(ui_.resetButton, &QPushButton::clicked, this, &MainWindow::reset_button_clicked);

}

// This function starts to calculate the score based on the the player choices of paylines
// and shows the result on the Text Edit widget in each spin and at the end shows the final result
void MainWindow::reelStopped() {

    for ( Reel* reel: reels_) {
        if (reel->is_spinning()){
            return;
        }
    }

    // after finishing spinning making a vector of the fruits on the labels to make
    // calculation of the sore of each payline easier
    std::vector< std::pair<std::string,int> > fruits_vec;
    for ( Reel* reel: reels_) {
        for (unsigned i = 0; i < 3; i++) {
            std::string fruiti = reel->get_symbol(i);
            fruits_vec.push_back(std::make_pair(fruiti, frequencies.find(fruiti)->second));
        }
    }

    // The socre in each spin
    int score = 0;

    // calculation of the score for the middle payline which is selected by default.
    // if the three fruits at the middle payline are same, I give three times of the fruit frequency
    // value which we have it in the frequency map
    if (fruits_vec[1].first == fruits_vec[4].first and fruits_vec[4].first == fruits_vec[7].first ) {
        score = score + 3 * fruits_vec[1].second;
    }

    // calculation of the score if the FirstThird button is clicked.
    // At first I checked the first payline if the fruits are same then check the third pyaline if the fruits are same
    // then I give the 9 times of the maximum frequency value of the fruits in first and third paylines.
    // if just the fruits of one of the paylines are same then I give the 3 times of the frequency value of that fruit
    // as score.
    if ( first_third_button_) {
        if (fruits_vec[0].first == fruits_vec[3].first and fruits_vec[3].first == fruits_vec[6].first ) {
            if (fruits_vec[2].first == fruits_vec[5].first and fruits_vec[5].first == fruits_vec[8].first) {
                score =  score + 3 * 3 * std::max(fruits_vec[2].second, fruits_vec[0].second);
            } else {
                score =  score + 3 * fruits_vec[0].second;
            }
        } else if (fruits_vec[2].first == fruits_vec[5].first and fruits_vec[5].first == fruits_vec[8].first) {
            score =  score + 3 * fruits_vec[2].second;
        }
    }

    // If the Diagonal button is clicked then I compare the fruits in each diagonal, if those are the same
    // I give the player 3 times of the frequency value of the fruit as score.
    if ( diag_button_) {
        if (fruits_vec[0].first == fruits_vec[4].first and fruits_vec[4].first == fruits_vec[8].first ) {
            score = score + 3 * fruits_vec[0].second;
        } else if (fruits_vec[2].first == fruits_vec[4].first and fruits_vec[4].first == fruits_vec[6].first) {
            score =  score + 3 * fruits_vec[2].second;
        }
    }

    // this is used to decrease the credit in each cycle and makes the player
    // able to change the payline choices during the game and calculate the new
    // number of times of spinning based on the new price of each spinning time.
    int credit_decrease = 0;
    if (first_third_button_ and diag_button_) {
        credit_decrease = 27;
    } else if ((first_third_button_ and not diag_button_) or (not first_third_button_ and diag_button_)) {
        credit_decrease = 9;
    } else {
        credit_decrease = 3;
    }

    credit_ = credit_ - credit_decrease;
    // add the new score to the previous total score the player has from the previous spinnings
    score_total_ = score_total_ + score;

    // decrease the number of times the player can click spin button
    spin_remained_ -= 1;

    // display the number of the spin remained
    ui_.spinRemained->display(spin_remained_);

    // if spin is equal to zero
    if ( spin_remained_ <= 0) {

        // If the score is equal to zero the player is lost the game.
        // I show the amount of the credits the player spent and the total score s/he gained
        // and the instruction of the play again or leave the game.
        // If the player has gain any score except zero in the game, s/he won the game and I
        // show the rest of the things I mentioned previously.
        if (score_total_ == 0) {
            ui_.result->clear();
            ui_.result->setPlainText(" You lost! Your credit was " + QString::number(ui_.enteryCredit->text().toInt()) + " and Your score is " +
                                     QString::number(score_total_) +". If you want to play again click Reset button, "
                                                                    "otherwise click Quit button.");
        } else {
            ui_.result->clear();
            ui_.result->setPlainText(" You won! Your credit was " + QString::number(ui_.enteryCredit->text().toInt()) + " and Your score is " +
                                     QString::number(score_total_) +". If you want to play again click Reset button, "
                                                                    "otherwise click Quit button.");
        }
        // Here I disable some of the buttons to guide the player to play again or leave.
        no_spin_remained();
        return;
    }

    // diplaying the score
    ui_.result->setPlainText("Your score is " + QString::number(score_total_));
}

// this is the method which is called after clicking the spin button
// and starts spin for each reel
void MainWindow::spin_clicked() {
    for ( Reel* reel: reels_){
        reel->init_spin();
    }
}

// this is the method which is called after clicking the Credit button
// if the spin remained is equal to zero it means this is the first time it wants to
// calculate the spinning number, but if the spin remained is not equal to zero means it is
// in the middle of the game and calculate the number of the remaining spinning based on the
// amount of the credit which remained during the game.
// because in each spinning time I decrease the amount of the original credit based on the
// number of paylines that the player has chosen.
void MainWindow::credit_entery_clicked() {
    if (spin_remained_ == 0) {
        credit_ = ui_.enteryCredit->text().toInt();
    }


    if (not diag_button_ and not first_third_button_){
        spin_remained_ = credit_/3;
    } else if ( not diag_button_ and first_third_button_ ){
        spin_remained_ = credit_/9;
    } else if ( diag_button_ and not first_third_button_) {
        spin_remained_ = credit_/9;
    } else if ( diag_button_ and first_third_button_ ){
        spin_remained_ = credit_/27;
    }
    ui_.spinRemained->display(spin_remained_);
}

// this is the method which is called after clicking the FirstThird button
void MainWindow::first_third_rows() {
    if (not first_third_button_) {
        first_third_button_ = true;
    } else {
        first_third_button_ = false;
    }
}

// this is the method which is called after clicking the Diagonal button
void MainWindow::diagonal() {
    if (not diag_button_) {
        diag_button_ = true;
    } else {
        diag_button_ = false;
    }
}

// this is the method which is called after clicking the Reset button
// this method resets everything.
void MainWindow::reset_button_clicked() {
    credit_ = 0;
    spin_remained_ = 0;
    score_total_ = 0;
    first_third_button_ = false;
    diag_button_ = false;
    ui_.spinButton->setEnabled(true);
    ui_.firstThird->setEnabled(true);
    ui_.firstThird->setChecked(false);
    ui_.diagonalButton->setEnabled(true);
    ui_.diagonalButton->setChecked(false);
    ui_.creditEntery->setEnabled(true);
    ui_.result->clear();
    ui_.enteryCredit->setText("Enter credit...");
    ui_.spinRemained->display(0);
}

// this is the method which is called after finishing the number of
// spinning and disabled all the buttons except Reset and Quit buttons.
void MainWindow::no_spin_remained() {
    ui_.spinButton->setEnabled(false);
    ui_.firstThird->setEnabled(false);
    ui_.diagonalButton->setEnabled(false);
    ui_.creditEntery->setEnabled(false);
}



