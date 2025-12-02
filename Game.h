#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "AI.h"

enum class GameMode {
    MainMenu,
    PVP,
    PVAI,
    GameOver
};

class Game {
public:
    Game();
    void run();
    bool isRunning() const;
    void handleInput();
    void update();
    Board& getBoard();
    GameMode getMode() const;
    bool isGameOver() const;
    std::string getGameResult() const;

private:
    Board board;
    AI ai;
    GameMode mode;
    bool running;
    bool gameOver;
    std::string gameResult;
    int selectedRow;
    int selectedCol;
    bool pieceSelected;

    void selectMode(GameMode newMode);
    void handlePieceSelection(int row, int col);
    void checkGameEnd();
};

#endif