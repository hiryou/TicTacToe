/**
 * Author:      Long Nguyen
 * Project:     CS300 - Assignment 4
 * License:     MIT license, see COPYING
 *
 * Copyright (C) 2010 Long Nguyen
 */

#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QAction>
#include <QResizeEvent>
#include "Window.hh"
#include "Button.hh"
#include "Board.hh"

Window::Window(QWidget *parent) : QMainWindow(parent) {
    // create qwidget containing game window
    QWidget *temp = new QWidget(this);
    setCentralWidget(temp);

    // Window title and size
    setWindowTitle("Tic-Tac-Toe");
    setMinimumSize(650, 500);

    // Centralize game window to user screen
    move(QApplication::desktop()->width() / 2 - (width() / 2),
         QApplication::desktop()->height() / 2 - (height() / 2));

    // create container frame holding game elements
    this->_container = new QFrame(temp);
    this->_container->setFixedSize(650, 500);
    
    // game board and the 3 buttons
    setupBoard();
    setupButtons();

    // state changes on board -> signal game state on main window
    connect(this->_board, SIGNAL(signalStateChange(GameState)),
                    SLOT(boardStateChange(GameState)));

    // when a player place his piece -> signal player go on main window
    connect(this->_board, SIGNAL(signalPlayerGo(int, int, Player)),
                    SLOT(boardPlayerGo(int, int, Player)));

    // when button X is clicked -> set X as the next player to go
    connect(this->_buttonX, SIGNAL(clicked()), SLOT(setCurrentTurnX()));
    // when button O is clicked -> set O as the next player to go
    connect(this->_buttonO, SIGNAL(clicked()), SLOT(setCurrentTurnO()));
    // when button New Game is clicked -> clear the board and create a new game
    connect(this->_buttonNewGame, SIGNAL(clicked()), this->_board, SLOT(newGame()));
}

void Window::boardStateChange(GameState newState) {
    // new game trigger: enable button X, O, and New Game
    if (newState == StateNewGame) {
        this->_buttonO->setEnabled(true);
        this->_buttonX->setEnabled(true);
        this->_buttonNewGame->setEnabled(true);
    }

    // winner or draw trigger: disable button X and O, enable button New Game
    else if (newState == StateWinnerX || newState == StateWinnerO
               || newState == StateDraw) {
        this->_buttonX->setEnabled(false);
        this->_buttonO->setEnabled(false);
        this->_buttonNewGame->setEnabled(true);
    }
}

void Window::boardPlayerGo(int, int, Player player) {
    // auto set the next turn to the player other than the recent played
    if (player == PlayerO)
        setCurrentTurnX();
    else
        setCurrentTurnO();
}

void Window::setCurrentTurnX() {
    this->_board->setCurrentTurnX();

    this->_buttonX->setChecked(true);
    this->_buttonO->setChecked(false);
}

void Window::setCurrentTurnO() {
    this->_board->setCurrentTurnO();

    this->_buttonO->setChecked(true);
    this->_buttonX->setChecked(false);
}

void Window::resizeEvent(QResizeEvent* e) {
    QMainWindow::resizeEvent(e);

    int newX = qMax(0, (e->size().width() - 650) / 2);
    int newY = qMax(0, (e->size().height() - 500) / 2);

    this->_container->move(newX, newY);
}

void Window::setupBoard() {
    this->_board = new Board(this->_container);
    this->_board->move(25, 25);
    this->_board->resize(450, 450);
}

void Window::setupButtons() {
    this->drawButtonX();
    this->drawButtonO();
    this->drawButtonNewGame();
}

void Window::drawButtonX() {
    this->_buttonX = new Button(this->_container);
    this->_buttonX->resize(128, 128);
    this->_buttonX->move(500, 270);
    this->_buttonX->setCheckedImage(QImage(":/images/x-color.png"));
    this->_buttonX->setDefaultImage(QImage(":/images/x-solid.png"));
    this->_buttonX->setEnabled(true);
    this->_buttonX->setCheckable(true);

    this->_buttonX->setChecked(true);
}

void Window::drawButtonO() {
    this->_buttonO = new Button(this->_container);
    this->_buttonO->resize(128, 128);
    this->_buttonO->move(500, 370);
    this->_buttonO->setCheckedImage(QImage(":/images/o-color.png"));
    this->_buttonO->setDefaultImage(QImage(":/images/o-solid.png"));
    this->_buttonO->setEnabled(true);
    this->_buttonO->setCheckable(true);
}

void Window::drawButtonNewGame() {
    this->_buttonNewGame = new Button(this->_container);
    this->_buttonNewGame->resize(128, 70);
    this->_buttonNewGame->move(500, 20);
    this->_buttonNewGame->setDefaultImage(QImage(":/images/new-game.png"));
    this->_buttonNewGame->setEnabled(true);
}
