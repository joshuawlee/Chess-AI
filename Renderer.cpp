#include "Renderer.h"

Renderer::Renderer() {}

bool Renderer::loadAssets() {
    // Load font (SFML 3.0.2 uses openFromFile instead of loadFromFile)
    // Try multiple font locations
    bool fontLoaded = false;
    
    // Try assets folder first
    if (font.openFromFile("assets/font.ttf")) {
        fontLoaded = true;
    }
    // Try macOS system fonts
    else if (font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        fontLoaded = true;
    }
    else if (font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        fontLoaded = true;
    }
    
    if (!fontLoaded) {
        return false;
    }
    
    // Load textures
    std::map<std::string, std::string> pieceMap = {
        {"wp", "white_pawn"}, {"wn", "white_knight"}, {"wb", "white_bishop"},
        {"wr", "white_rook"}, {"wq", "white_queen"}, {"wk", "white_king"},
        {"bp", "black_pawn"}, {"bn", "black_knight"}, {"bb", "black_bishop"},
        {"br", "black_rook"}, {"bq", "black_queen"}, {"bk", "black_king"}
    };
    
    for (const auto& pair : pieceMap) {
        if (!textures[pair.first].loadFromFile("assets/" + pair.second + ".png")) {
            return false;
        }
    }
    
    return true;
}

std::string Renderer::pieceKey(PieceType type, PieceColor color) {
    std::string key;
    
    if (color == PieceColor::White) {
        key = "w";
    } else if (color == PieceColor::Black) {
        key = "b";
    } else {
        return "";
    }
    
    switch (type) {
        case PieceType::Pawn: key += "p"; break;
        case PieceType::Knight: key += "n"; break;
        case PieceType::Bishop: key += "b"; break;
        case PieceType::Rook: key += "r"; break;
        case PieceType::Queen: key += "q"; break;
        case PieceType::King: key += "k"; break;
        default: return "";
    }
    
    return key;
}

void Renderer::drawBoard(sf::RenderWindow& window) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            // SFML 3: setPosition takes Vector2f
            tile.setPosition(sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE));
            
            if ((row + col) % 2 == 0) {
                tile.setFillColor(sf::Color(240, 217, 181)); // Light square
            } else {
                tile.setFillColor(sf::Color(181, 136, 99)); // Dark square
            }
            
            window.draw(tile);
        }
    }
}

void Renderer::drawPieces(sf::RenderWindow& window, const Board& board) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece piece = board.squares[row][col];
            if (piece.isEmpty()) continue;
            
            std::string key = pieceKey(piece.type, piece.color);
            if (key.empty() || textures.find(key) == textures.end()) continue;
            
            // SFML 3: Sprite must be constructed with texture
            sf::Sprite sprite(textures[key]);
            // SFML 3: setPosition takes Vector2f
            sprite.setPosition(sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE));
            
            // Scale sprite to fit tile
            sf::Vector2u textureSize = textures[key].getSize();
            float scaleX = TILE_SIZE / (float)textureSize.x;
            float scaleY = TILE_SIZE / (float)textureSize.y;
            // SFML 3: setScale takes Vector2f
            sprite.setScale(sf::Vector2f(scaleX, scaleY));
            
            window.draw(sprite);
        }
    }
}

void Renderer::drawSelection(sf::RenderWindow& window, int row, int col) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return;
    
    sf::RectangleShape highlight(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    // SFML 3: setPosition takes Vector2f
    highlight.setPosition(sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE));
    highlight.setFillColor(sf::Color(255, 255, 0, 100)); // Yellow transparent
    window.draw(highlight);
}

void Renderer::renderGame(sf::RenderWindow& window, const Board& board,
                          int selectedRow, int selectedCol, bool pieceSelected) {
    drawBoard(window);
    
    if (pieceSelected) {
        drawSelection(window, selectedRow, selectedCol);
    }
    
    drawPieces(window, board);
}

void Renderer::renderMainMenu(sf::RenderWindow& window) {
    window.clear(sf::Color(40, 40, 40));
    
    // SFML 3: Text constructor is Text(font, string, characterSize)
    sf::Text title(font, "Chess Game", 60);
    title.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = title.getLocalBounds();
    // SFML 3: Use size.x instead of width
    title.setPosition(sf::Vector2f((800 - titleBounds.size.x) / 2, 150));
    window.draw(title);
    
    // PVP Button
    sf::RectangleShape pvpButton(sf::Vector2f(300, 60));
    pvpButton.setPosition(sf::Vector2f(250, 350));
    pvpButton.setFillColor(sf::Color(70, 130, 180));
    window.draw(pvpButton);
    
    sf::Text pvpText(font, "Player vs Player", 30);
    pvpText.setFillColor(sf::Color::White);
    sf::FloatRect pvpBounds = pvpText.getLocalBounds();
    pvpText.setPosition(sf::Vector2f(250 + (300 - pvpBounds.size.x) / 2, 365));
    window.draw(pvpText);
    
    // PVAI Button
    sf::RectangleShape pvaiButton(sf::Vector2f(300, 60));
    pvaiButton.setPosition(sf::Vector2f(250, 450));
    pvaiButton.setFillColor(sf::Color(70, 130, 180));
    window.draw(pvaiButton);
    
    sf::Text pvaiText(font, "Player vs AI", 30);
    pvaiText.setFillColor(sf::Color::White);
    sf::FloatRect pvaiBounds = pvaiText.getLocalBounds();
    pvaiText.setPosition(sf::Vector2f(250 + (300 - pvaiBounds.size.x) / 2, 465));
    window.draw(pvaiText);
}

void Renderer::renderGameOver(sf::RenderWindow& window, const std::string& result) {
    window.clear(sf::Color(40, 40, 40));
    
    // Result text
    sf::Text resultText(font, result, 50);
    resultText.setFillColor(sf::Color::White);
    sf::FloatRect resultBounds = resultText.getLocalBounds();
    resultText.setPosition(sf::Vector2f((800 - resultBounds.size.x) / 2, 250));
    window.draw(resultText);
    
    // Return button
    sf::RectangleShape button(sf::Vector2f(300, 60));
    button.setPosition(sf::Vector2f(250, 450));
    button.setFillColor(sf::Color(70, 130, 180));
    window.draw(button);
    
    sf::Text buttonText(font, "Return to Menu", 30);
    buttonText.setFillColor(sf::Color::White);
    sf::FloatRect buttonBounds = buttonText.getLocalBounds();
    buttonText.setPosition(sf::Vector2f(250 + (300 - buttonBounds.size.x) / 2, 465));
    window.draw(buttonText);
}

void Renderer::renderPauseMenu(sf::RenderWindow& window) {
    // Semi-transparent dark overlay
    sf::RectangleShape overlay(sf::Vector2f(800, 800));
    overlay.setPosition(sf::Vector2f(0, 0));
    overlay.setFillColor(sf::Color(0, 0, 0, 180)); // Black with transparency
    window.draw(overlay);
    
    // Pause title
    sf::Text title(font, "PAUSED", 60);
    title.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(sf::Vector2f((800 - titleBounds.size.x) / 2, 150));
    window.draw(title);
    
    // Resume button
    sf::RectangleShape resumeButton(sf::Vector2f(300, 60));
    resumeButton.setPosition(sf::Vector2f(250, 300));
    resumeButton.setFillColor(sf::Color(70, 130, 180));
    window.draw(resumeButton);
    
    sf::Text resumeText(font, "Resume Game", 30);
    resumeText.setFillColor(sf::Color::White);
    sf::FloatRect resumeBounds = resumeText.getLocalBounds();
    resumeText.setPosition(sf::Vector2f(250 + (300 - resumeBounds.size.x) / 2, 315));
    window.draw(resumeText);
    
    // Return to Menu button
    sf::RectangleShape menuButton(sf::Vector2f(300, 60));
    menuButton.setPosition(sf::Vector2f(250, 400));
    menuButton.setFillColor(sf::Color(180, 70, 70)); // Reddish color
    window.draw(menuButton);
    
    sf::Text menuText(font, "Return to Menu", 30);
    menuText.setFillColor(sf::Color::White);
    sf::FloatRect menuBounds = menuText.getLocalBounds();
    menuText.setPosition(sf::Vector2f(250 + (300 - menuBounds.size.x) / 2, 415));
    window.draw(menuText);
    
    // ESC hint
    sf::Text escHint(font, "Press ESC to resume", 20);
    escHint.setFillColor(sf::Color(200, 200, 200));
    sf::FloatRect escBounds = escHint.getLocalBounds();
    escHint.setPosition(sf::Vector2f((800 - escBounds.size.x) / 2, 550));
    window.draw(escHint);
}