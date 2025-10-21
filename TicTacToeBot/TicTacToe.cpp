#include "TicTacToe.h"

TicTacToeGame::TicTacToeGame() : board(9, ' '), currentPlayer('X'), winner(' '), gameOver(false) {}

bool TicTacToeGame::makeMove(int position) {
    if (position < 0 || position >= 9 || board[position] != ' ' || gameOver) {
        return false;
    }
    board[position] = currentPlayer;
    if (checkWin()) {
        winner = currentPlayer;
        gameOver = true;
    } else if (checkDraw()) {
        winner = 'D'; // 'D' for Draw
        gameOver = true;
    } else {
        switchPlayer();
    }
    return true;
}

bool TicTacToeGame::isGameOver() const {
    return gameOver;
}

char TicTacToeGame::getCurrentPlayer() const {
    return currentPlayer;
}

char TicTacToeGame::getWinner() const {
    return winner;
}

std::string TicTacToeGame::getStatus() {
    if (gameOver) {
        if (winner == 'D') {
            return "It's a draw!";
        }
        return "Player " + std::string(1, winner) + " wins!";
    }
    return "Player " + std::string(1, currentPlayer) + "'s turn";
}

const std::vector<char>& TicTacToeGame::getBoardState() const {
    return board;
}

void TicTacToeGame::switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

bool TicTacToeGame::checkWin() {
    const int win_conditions[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };

    for (int i = 0; i < 8; ++i) {
        if (board[win_conditions[i][0]] == currentPlayer &&
            board[win_conditions[i][1]] == currentPlayer &&
            board[win_conditions[i][2]] == currentPlayer) {
            return true;
        }
    }
    return false;
}

bool TicTacToeGame::checkDraw() {
    for (int i = 0; i < 9; ++i) {
        if (board[i] == ' ') {
            return false;
        }
    }
    return true;
}

