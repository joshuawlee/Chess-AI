#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Board.h"
#include "AI.h"
#include "Renderer.h"

enum class GameMode { MainMenu, PVP, PVAI, GameOver };

class Game {
public:
    Game();
    void run();
    
private:
    Board board;
    AI ai;
    Renderer renderer;
    GameMode mode;
    
    bool gameOver;
    std::string resultText;
    
    int selectedRow;
    int selectedCol;
    bool pieceSelected;
    
    void handleInput(sf::RenderWindow& window);
    void update();
    void render(sf::RenderWindow& window);
    
    void resetGame();
    void selectGameMode(GameMode newMode);
    void handleBoardClick(int row, int col);
    void checkGameEndState();
    void handleAITurn();
};

#endif // GAME_H