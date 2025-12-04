#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Board.h"

class Renderer {
public:
    Renderer();
    
    bool loadAssets();
    void renderGame(sf::RenderWindow& window, const Board& board,
                    int selectedRow, int selectedCol, bool pieceSelected);
    void renderMainMenu(sf::RenderWindow& window);
    void renderGameOver(sf::RenderWindow& window, const std::string& result);
    
    static const int TILE_SIZE = 100;
    
private:
    std::map<std::string, sf::Texture> textures;
    sf::Font font;
    
    void drawBoard(sf::RenderWindow& window);
    void drawPieces(sf::RenderWindow& window, const Board& board);
    void drawSelection(sf::RenderWindow& window, int row, int col);
    
    std::string pieceKey(PieceType type, PieceColor color);
};

#endif // RENDERER_H