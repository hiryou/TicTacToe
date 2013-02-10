/**
 * Author:      Long Nguyen
 * Project:     CS300 - Assignment 4
 * License:     MIT license, see COPYING
 *
 * Copyright (C) 2010 Long Nguyen
 */

#include <QPainter>
#include <QMouseEvent>
#include "Board.hh"

Board::Board(QWidget *parent) : QWidget(parent),
      _currentGameState(StatePlaying),
      _currentTurn(PlayerX),

      _imgCellBg(QImage(":/images/cell-bg.png")),
      _imgBoardBg(QImage(":/images/board-bg.jpg")),

      _imgHoverX(QImage(":/images/x-solid.png")),
      _imgHoverO(QImage(":/images/o-solid.png")),
      _imgPlayX(QImage(":/images/x-color.png")),
      _imgPlayO(QImage(":/images/o-color.png")),
      _imgWinX(QImage(":/images/x-win.png")),
      _imgWinO(QImage(":/images/o-win.png")),
      _imgDraw(QImage(":/images/draw.png")) {
    // For hovers:
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    newGame();
}

void Board::newGame() {
    for (int i = 0; i < 9; i++)
        this->_board[i] = NoPlayer;
    this->_currentGameState = StatePlaying;
    update();

    // Signal a redraw due to state change (new game)
    emit signalStateChange(StateNewGame);
}

void Board::setCurrentTurnX() {
    this->_currentTurn = PlayerX;
}

void Board::setCurrentTurnO() {
    this->_currentTurn = PlayerO;
}

Player Board::getCurrentTurn() const {
    return this->_currentTurn;
}

GameState Board::getCurrentGameState() const {
    return this->_currentGameState;
}

/************************************************************
  * Protected Methods
  */

void Board::paintEvent(QPaintEvent* ) {
    // calculate cell width and cell height
    const qreal thirdW = width()/3.0;
    const qreal thirdH = height()/3.0;

    // prepare painter
    QPainter painter(this);

    // draw board bg
    const QRectF rect0(0, 0, thirdW*3, thirdH*3);
    painter.drawImage(rect0, this->_imgBoardBg);

    // fill cell-bg to each cell
    const QRectF rect1(0, 0, thirdW, thirdH);
    painter.drawImage(rect1, this->_imgCellBg);
    const QRectF rect2(thirdW, 0, thirdW, thirdH);
    painter.drawImage(rect2, this->_imgCellBg);
    const QRectF rect3(thirdW*2, 0, thirdW, thirdH);
    painter.drawImage(rect3, this->_imgCellBg);
    const QRectF rect4(0, thirdH, thirdW, thirdH);
    painter.drawImage(rect4, this->_imgCellBg);
    const QRectF rect5(thirdW, thirdH, thirdW, thirdH);
    painter.drawImage(rect5, this->_imgCellBg);
    const QRectF rect6(thirdW*2, thirdH, thirdW, thirdH);
    painter.drawImage(rect6, this->_imgCellBg);
    const QRectF rect7(0, thirdH*2, thirdW, thirdH);
    painter.drawImage(rect7, this->_imgCellBg);
    const QRectF rect8(thirdW, thirdH*2, thirdW, thirdH);
    painter.drawImage(rect8, this->_imgCellBg);
    const QRectF rect9(thirdW*2, thirdH*2, thirdW, thirdH);
    painter.drawImage(rect9, this->_imgCellBg);

    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform);

    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++) {
            const int index = row * 3 + col; // Internal index
            // draw a rectangle:
            const QRectF rect(thirdW * col, thirdH * row, thirdW, thirdH);
            // player state in current index:
            const Player player = this->_board[index];

            if (player == NoPlayer) {
                if (this->_hoverIndex == index) {
                    if (this->_currentTurn == PlayerX)
                        painter.drawImage(rect, this->_imgHoverX);
                    else
                        painter.drawImage(rect, this->_imgHoverO);
                }
            }
            else if (player == PlayerX)
                painter.drawImage(rect, this->_imgPlayX);
            else if (player == PlayerO)
                painter.drawImage(rect, this->_imgPlayO);
        }

    // check for win/draw condition
    if (this->_currentGameState != StatePlaying) {
        const qreal marginW = width() * 0.10; // 10% margins
        const qreal marginH = height() * 0.10;
        const QRectF rect(marginW, marginH,
                          width() - 2 * marginW, height() - 2 * marginH);

        if (this->_currentGameState == StateWinnerO)
            painter.drawImage(rect, this->_imgWinO);
        else if (this->_currentGameState == StateWinnerX)
            painter.drawImage(rect, this->_imgWinX);
        else if (this->_currentGameState == StateDraw)
            painter.drawImage(rect, this->_imgDraw);
    }
}

void Board::leaveEvent(QEvent *) {
    if (this->_hoverIndex != -1) {
        this->_hoverIndex = -1;
        update(); // trigger a redraw
    }
}

void Board::mouseMoveEvent(QMouseEvent* e) {
    if (this->_currentGameState != StatePlaying)
        return;

    int index = getIndexFromCoords(e->pos());

    if (this->_hoverIndex != index) {
        // check which index is being hovered
        if (index != -1 && this->_board[index] == NoPlayer)
            this->_hoverIndex = index;
        else
            this->_hoverIndex = -1;
        update();
    }
}

void Board::mousePressEvent(QMouseEvent* e) {
    if (this->_currentGameState == StateWinnerX
        || this->_currentGameState == StateWinnerO
        || this->_currentGameState == StateDraw) {
        // click anywhere on the board to create a new game
        e->accept();
        this->newGame();
    } else if (this->_currentGameState == StatePlaying) {
        e->accept();
        int index = getIndexFromCoords(e->pos());

        Q_ASSERT(index != -1);

        if (this->_board[index] == NoPlayer)
            playInIndex(index, this->_currentTurn);
    }
}

/**************************************************
  * Private Methods
  */

void Board::playInIndex(int pos, Player player) {
    Q_ASSERT(pos >= 0 && pos < 9);

    GameState oldState = this->_currentGameState;

    this->_board[pos] = player;
    updateGameState();
    update(); // redraw

    emit signalPlayerGo(pos % 3, pos / 3, player);

    if (oldState != this->_currentGameState)
        emit signalStateChange(this->_currentGameState);
}

GameState Board::checkThree(Player c1, Player c2, Player c3) const {
    /**
      * how it works??
      *
      * remind: 1 = X, 2 = O, 3 = empty cell
      *
      * (in binary)
      * 1 = 01
      * 2 = 10
      * 3 = 11
      *
      * 01 & 01 & 01 = 1 = [X, X, X] : X wins
      * 10 & 10 & 10 = 2 = [O, O, O] = O wins
      * 11 & 11 & 11 = 3 = [_, _, _] = game in progress
      *
      * 01 & 01 & 10 = 0 = [X, X, O] = draw
      * 01 & 01 & 11 = 1 = [X, X, _] = game in progress
      * 10 & 10 & 01 = 0 = [O, O, X] = draw
      * 10 & 10 & 11 = 2 = [O, O, _] = game in progress
      * 11 & 11 & 01 = 1 = [X, _, _] = game in progress
      * 11 & 11 & 10 = 2 = [0, _, _] = game in progress
      *
      * 01 & 10 & 11 = 0 = [X, O, _] = draw (easily predictable)
      *
      */

    // check if state draw
    if ((c1 & c2 & c3) == 0)
        return StateDraw;

    // efficient method to check either X|O wins
    int sum = c1 + c2 + c3;
    // if all X
    if (sum == PlayerX * 3)
        return StateWinnerX;
    // else if all O
    else if (sum == PlayerO * 3)
        return StateWinnerO;

    // remain cases are state playing or game still in progress
    return StatePlaying;
}

GameState Board::checkCurrentGameState() {
    /**
      * how it works??
      *
      * 1. remind: Board::checkThree returns either draw|playing|Xwins|Owins
      *   0 = 000 = draw
      *   1 = 001 = playing
      *   2 = 010 = Xwins
      *   4 = 100 = Owins
      * - and we have 8 entity of value restricted to the above set due to the
      *   fact of 3 rows, 3 cols and 2 diagonals
      *
      * 2. thank to the nature of the game process, we have, in terms of 8 values:
      *   - X wins iff the set of 8 values consists of
      *     ([X] means set of 1 or more x.
      *      e.g. [Xwins] means the set of 1 or more straigh line of X)
      *     + [Xwins] && [draw]: 2 | 0 = 2
      *     + [Xwins] && [playing]: 2 | 1 = 3
      *     + [Xwins] && [draw] && [playing]: 2 | 0 | 1 = 3
      *   - O wins iff the set of 8 values consists of
      *     + [Owins] && [draw]: 4 | 0 = 4
      *     + [Owins] && [playing]: 4 | 1 = 5
      *     + [Owins] && [draw] && [playing]: 4 | 0 | 1 = 5
      *   - playing iff the set of 8 values consists of
      *     + [playing]: 1 = 1
      *     + [playing] && [draw]: 1 | 0 = 1
      *   - otherwise, the current game state is draw
      */

    // remember checkTree on 3 rows, 3 cols and 2 diagonals
    int lines[8], stateReturn;

    // 3 rows
    lines[0] = checkThree(this->_board[0], this->_board[1], this->_board[2]);
    lines[1] = checkThree(this->_board[3], this->_board[4], this->_board[5]);
    lines[2] = checkThree(this->_board[6], this->_board[7], this->_board[8]);

    // 3 cols
    lines[3] = checkThree(this->_board[0], this->_board[3], this->_board[6]);
    lines[4] = checkThree(this->_board[1], this->_board[4], this->_board[7]);
    lines[5] = checkThree(this->_board[2], this->_board[5], this->_board[8]);

    // 2 diagonals
    lines[6] = checkThree(this->_board[0], this->_board[4], this->_board[8]);
    lines[7] = checkThree(this->_board[2], this->_board[4], this->_board[6]);

    // decide game state
    stateReturn = lines[0] | lines[1] | lines[2] | lines[3] |
                lines[4] | lines[5] | lines[6] | lines[7];

    // check if X wins
    if (stateReturn == StateWinnerX || stateReturn == StateWinnerX+StatePlaying) {
        return StateWinnerX;
    }
    // check if O wins
    if (stateReturn == StateWinnerO || stateReturn == StateWinnerO+StatePlaying) {
        return StateWinnerO;
    }
    // check if playing
    if (stateReturn == StatePlaying) {
        return StatePlaying;
    }
    // otherwise, it is a draw
    else {
        return StateDraw;
    }
}

void Board::updateGameState() {
    // update current gate state to the latest value
    this->_currentGameState = this->checkCurrentGameState();
}

int Board::getIndexFromCoords(const QPoint &pos) const {
    const int x = pos.x();
    const int y = pos.y();
    const int w = width();
    const int h = height();
    if (x < 0 || x >= w || y < 0 || y >= h) // out of bounds
        return -1;
    return (int(x / (w / 3.0)) + 3 * int(y / (h / 3.0)));
}
