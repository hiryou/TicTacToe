/**
 * Author:      Long Nguyen
 * Project:     CS300 - Assignment 4
 * License:     MIT license, see COPYING
 *
 * Copyright (C) 2010 Long Nguyen
 */

#ifndef WINDOW_HH

#define WINDOW_HH

#include <QMainWindow>
#include <QTextStream>
#include <QFrame>
#include "Button.hh"
#include "Board.hh"

class Window : public QMainWindow {
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);

public slots:
    /**
     * receive signal from board when game state changes
     */
    void boardStateChange(GameState state);

    /**
     * receive signal from board when a player goes
     */
    void boardPlayerGo(int col, int row, Player player);

    /**
     * receive signal when button X is clicked
     * - set current turn to player X
     */
    void setCurrentTurnX();

    /**
     * receive signal when button O is clicked
     * - set current turn to player O
     */
    void setCurrentTurnO();

    void resizeEvent(QResizeEvent* e);

private:
    // set up board
    void setupBoard();
    // set up buttons
    void setupButtons();

    // draw button X
    void drawButtonX();
    // draw button O
    void drawButtonO();
    // draw button New Game
    void drawButtonNewGame();

    Board*      _board;
    Button*     _buttonX;
    Button*     _buttonO;
    Button*     _buttonNewGame;
    QFrame*     _container;
};

#endif // WINDOW_HH
