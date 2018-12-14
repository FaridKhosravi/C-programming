/*
#####################################################################
# TIE-02207 Programming 2: Basics, F2018                            #
# Project4: Slots                                                   #
# Program description: Implements a virtual slots game.             #
# File: main_window.h                                               #
# Description: Implements a UI for the game to be playable.         #
# Notes: * This is an exercise program.                             #
#        * Students need to modify this file.                       #
#####################################################################
*/
// This is the fourth Project of Programming 2: Basics TIE-02207
// Farid Khosravi - Student Number: 267964 - Email: farid.khosravi@student.tut.fi
// Class MainWindow is a child of QMainWindow with some method and private slot
// function and private variables. This is the header file of this class.
// The purpose of this class is to manage the main window of the ui and clarify
// the connection between each widget and methods.



#ifndef SLOTS_MAINWINDOW_H
#define SLOTS_MAINWINDOW_H

#include "reel.hh"
#include "ui_main_window.h"
#include <QMainWindow>
#include <string>
#include <QPalette>


// class MainWindow
// The main window is used to interact with the game.
//
// MainWindow owns all the widgets used to display the game to the player
// and to play the game. MainWindow contains the whole game, no other windows
// are used.

class MainWindow: public QMainWindow {
    Q_OBJECT

public:

    // Construct a new MainWindow.
    //
    //parent Points to this MainWindow's parent widget.
    explicit MainWindow(QWidget* parent = nullptr);

    // When spin button is clicked this method is called
    void spin_clicked();

    // after clicking the Credit button this method is called
    void credit_entery_clicked();

    // when the player clicks on FirstThird button this method will call
    void first_third_rows();

    // when the player clicks on Diagonal button this method will call
    void diagonal();

    // when the player clicks on Reset button this method will call
    void reset_button_clicked();

    // when there is no spin times, this method disable some part of buttons
    void no_spin_remained();

private slots:

    // after stop spinning this method is called and calculate the score of the player
    void reelStopped();

private:

    //Initialize the Reels, buttons, etc. so the game can be played.
    void initUi();



    const Fruits fruits_;       // The game symbols and their rarities.
    Ui::MainWindowClass ui_;    // (Black) Qt magic handling the UI.

    // The private variable names are clear I thinkپ
    bool first_third_button_ = false;
    bool diag_button_ = false;

    int score_total_ = 0;
    int spin_remained_ = 0;
    int credit_ = 0;

    // Contains each Reel object pointers
    std::vector<Reel*> reels_;
};  // class MainWindow


#endif  // SLOTS_MAINWINDOW_H
