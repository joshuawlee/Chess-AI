#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Game.h"
#include <map>
#include <string>

class Renderer {
public:
    Renderer();
    ~Renderer();
    bool loadAssets();
    void render(sf::RenderWindow& window, Game& game, int selectedRow, int selectedCol, bool pieceSelected);
    void renderBoard(sf::RenderWindow& window);
    void renderPieces(sf::RenderWindow& window, const Board& board);
    void renderSelection(sf::RenderWindow& window, int row, int col);
    void renderMainMenu(sf::RenderWindow& window);
    void renderGameOver(sf::RenderWindow& window, const std::string& result);

    static const int TILE_SIZE = 100;
    static const int BOARD_SIZE = 800;

private:
    std::map<std::string, sf::Texture> textures;
    sf::Font font;
    std::string getPieceKey(PieceType type, PieceColor color);
};

#endif