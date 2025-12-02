#include "Renderer.h"
#include <iostream>

Renderer::Renderer() {}

Renderer::~Renderer() {}

std::string Renderer::getPieceKey(PieceType type, PieceColor color) {
    std::string colorStr = (color == PieceColor::White) ? "white" : "black";
    std::string typeStr;
    
    switch(type) {
        case PieceType::Pawn: typeStr = "pawn"; break;
        case PieceType::Knight: typeStr = "knight"; break;
        case PieceType::Bishop: typeStr = "bishop"; break;
        case PieceType::Rook: typeStr = "rook"; break;
        case PieceType::Queen: typeStr = "queen"; break;
        case PieceType::King: typeStr = "king"; break;
        default: return "";
    }
    
    return colorStr + "_" + typeStr;
}

bool Renderer::loadAssets() {
    // Load piece textures
    std::vector<std::string> pieceNames = {
        "white_pawn", "white_knight", "white_bishop", "white_rook", "white_queen", "white_king",
        "black_pawn", "black_knight", "black_bishop", "black_rook", "black_queen", "black_king"
    };
    
    for(const std::string& name : pieceNames) {
        sf::Texture texture;
        if(!texture.loadFromFile("assets/" + name + ".png")) {
            std::cerr << "Failed to load texture: assets/" << name << ".png" << std::endl;
            std::cerr << "Please ensure all piece images are in the assets/ folder." << std::endl;
            return false;
        }
        textures[name] = std::move(texture);
    }
    
    // Try to load font (optional, use default if fails)
    if(!font.openFromFile("assets/arial.ttf")) {
        std::cerr << "Warning: Could not load font. Using default font." << std::endl;
        // SFML will use default font
    }
    
    return true;
}

void Renderer::renderBoard(sf::RenderWindow& window) {
    sf::Color lightSquare(240, 217, 181);
    sf::Color darkSquare(181, 136, 99);
    
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            sf::RectangleShape square(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            square.setPosition(sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE));
            
            if((row + col) % 2 == 0) {
                square.setFillColor(lightSquare);
            } else {
                square.setFillColor(darkSquare);
            }
            
            window.draw(square);
        }
    }
}

void Renderer::renderPieces(sf::RenderWindow& window, const Board& board) {
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            Piece piece = board.getPiece(row, col);
            if(piece.type != PieceType::None) {
                std::string key = getPieceKey(piece.type, piece.color);
                if(textures.find(key) != textures.end()) {
                    sf::Sprite sprite(textures[key]);
                    
                    // Scale sprite to fit tile
                    sf::Vector2u textureSize = textures[key].getSize();
                    float scaleX = (float)TILE_SIZE / textureSize.x;
                    float scaleY = (float)TILE_SIZE / textureSize.y;
                    sprite.setScale(sf::Vector2f(scaleX, scaleY));
                    
                    sprite.setPosition(sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE));
                    window.draw(sprite);
                }
            }
        }
    }
}

void Renderer::renderSelection(sf::RenderWindow& window, int row, int col) {
    if(row >= 0 && row < 8 && col >= 0 && col < 8) {
        sf::RectangleShape highlight(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        highlight.setPosition(sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE));
        highlight.setFillColor(sf::Color(255, 255, 0, 100)); // Yellow with transparency
        window.draw(highlight);
    }
}

void Renderer::renderMainMenu(sf::RenderWindow& window) {
    window.clear(sf::Color(50, 50, 50));
    
    sf::Text title(font);
    title.setString("C++ Chess MVP");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(250, 150));
    window.draw(title);
    
    sf::Text pvpText(font);
    pvpText.setString("Press 1 for Player vs Player");
    pvpText.setCharacterSize(30);
    pvpText.setFillColor(sf::Color::White);
    pvpText.setPosition(sf::Vector2f(200, 350));
    window.draw(pvpText);
    
    sf::Text pvaiText(font);
    pvaiText.setString("Press 2 for Player vs AI");
    pvaiText.setCharacterSize(30);
    pvaiText.setFillColor(sf::Color::White);
    pvaiText.setPosition(sf::Vector2f(200, 400));
    window.draw(pvaiText);
}

void Renderer::renderGameOver(sf::RenderWindow& window, const std::string& result) {
    sf::RectangleShape overlay(sf::Vector2f(BOARD_SIZE, BOARD_SIZE));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    sf::Text resultText(font);
    resultText.setString(result);
    resultText.setCharacterSize(40);
    resultText.setFillColor(sf::Color::White);
    
    // Center the text
    sf::FloatRect textBounds = resultText.getLocalBounds();
    resultText.setPosition(sf::Vector2f((BOARD_SIZE - textBounds.size.x) / 2, 300));
    window.draw(resultText);
    
    sf::Text restartText(font);
    restartText.setString("Press ESC for Main Menu");
    restartText.setCharacterSize(25);
    restartText.setFillColor(sf::Color::White);
    
    sf::FloatRect restartBounds = restartText.getLocalBounds();
    restartText.setPosition(sf::Vector2f((BOARD_SIZE - restartBounds.size.x) / 2, 400));
    window.draw(restartText);
}

void Renderer::render(sf::RenderWindow& window, Game& game, int selectedRow, int selectedCol, bool pieceSelected) {
    window.clear();
    
    GameMode mode = game.getMode();
    
    if(mode == GameMode::MainMenu) {
        renderMainMenu(window);
    } else if(mode == GameMode::PVP || mode == GameMode::PVAI || mode == GameMode::GameOver) {
        renderBoard(window);
        renderPieces(window, game.getBoard());
        
        if(pieceSelected) {
            renderSelection(window, selectedRow, selectedCol);
        }
        
        if(mode == GameMode::GameOver) {
            renderGameOver(window, game.getGameResult());
        }
    }
    
    window.display();
}