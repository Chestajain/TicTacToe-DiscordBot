#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <vector>
#include <string>

class TicTacToeGame {
public:
    TicTacToeGame();
    bool makeMove(int position);
    bool isGameOver() const;
    char getCurrentPlayer() const;
    std::string getStatus();
    const std::vector<char>& getBoardState() const;
    char getWinner() const;

private:
    std::vector<char> board;
    char currentPlayer;
    char winner;
    bool gameOver;

    void switchPlayer();
    bool checkWin();
    bool checkDraw();
};

#endif // TICTACTOE_H

