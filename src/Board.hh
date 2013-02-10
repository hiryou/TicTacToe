/**
 * Author:      Long Nguyen
 * Project:     CS300 - Assignment 4
 * License:     MIT license, see COPYING
 *
 * Copyright (C) 2010 Long Nguyen
 */

#ifndef BOARD_HH

#define BOARD_HH

#include <QWidget>
#include <QPoint>

/**
  * - this enums use bitwise operation to efficiently calculate important game
  *   states on a given straigh line: draw|X wins|O wins|game in progress
  * - do not change these values unless you can ensure the bitwise operations
  *   in Board::checkThree remain the same results
  * - how to understand: consult Board::checkThree
  */
enum Player {
    PlayerO     = 1,
    PlayerX     = 2,
    NoPlayer    = 3,
};

/**
  * - this enums use bitwise operation to efficiently calculate important game
  *   states on the whole board: draw|X wins|O wins|game in progress
  * - do not change these values unless you can ensure the bitwise operations
  *   in Board::checkCurrentGameState remain the same results
  * - how to understand: consult Board::checkCurrentGameState
  */
enum GameState {
    StateDraw           = 0,
    StatePlaying        = 1,
    StateWinnerX        = 2,
    StateWinnerO        = 4,
    StateNewGame        = 8,
};

class Board : public QWidget {
    Q_OBJECT

public:
    explicit Board(QWidget *parent = 0);

    /**
     * get the current player turn, either X or O
     */
    Player getCurrentTurn() const;

    /**
     * get current game state
     */
    GameState getCurrentGameState() const;

public slots:
    /**
     * set current turn to player X
     */
    void setCurrentTurnX();

    /**
     * set current turn to player O
     */
    void setCurrentTurnO();
    
    /**
     * new game, create a new board
     */
    void newGame();

signals:
    /**
     * signal when a player X|O goes
     * @param int col 0|1|2
     * @param int row 0|1|2
     * @param Player player the one who goes
     */
    void signalPlayerGo(int col, int row, Player player);

    /**
     * signal when the game state changes
     * @param GameState newState the state to update to
     */
    void signalStateChange(GameState newState);

protected:
    // overridden Qt events
    void paintEvent(QPaintEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void leaveEvent(QEvent* e);

private:
    /**
     * place player X|O|nothing to location [0..8]; updates the
     * play state and emits a play state change if required.
     * @param index Position on board in col + row * 3 format [0..8]
     * @param player X|O|nothing
     */
    void playInIndex(int index, Player player);
    
    /**
     * update game state according to game state on board
     */
    void updateGameState();

    /**
     * return StateWinnerX|StateWinnerO|StatePlaying|StateDraw
     */
    GameState checkCurrentGameState();
    
    /**
      * check a row/col/diagonal to obtain what state it is draw|X wins|O wins
      * @param Player c1, c2, c3 on a straigh line (row/col/diagonal)
      * @return StateDraw if [X, O, O] or [O, X, X] regardless of appearing order
      *         StateWinnerX if [X, X, X]
      *         StateWinnerO if [O, O, O]
      *         StatePlaying for all remain cases
      */
    GameState checkThree(Player c1, Player c2, Player c3) const;
    
    /**
      * Returns index [0..8] from a given coordinate
      * @param pos a position s.t., 0 <= x < width(), 0 <= y < height()
      * @return the index [0..8], or -1 if a nonsense coords is provided
      */
    int getIndexFromCoords(const QPoint &pos) const;
                   
    // the game board in 1-dimension array
    Player _board[9];
    // current game state
    GameState _currentGameState;
    // the current turn!
    Player _currentTurn;
    // the index which is mouse hovered
    int _hoverIndex;

    // graphic images
    QImage _imgCellBg;
    QImage _imgBoardBg;

    // interact images
    QImage _imgHoverX;
    QImage _imgHoverO;
    QImage _imgPlayX;
    QImage _imgPlayO;
    QImage _imgWinX;
    QImage _imgWinO;
    QImage _imgDraw;
};

#endif // BOARD_HH
