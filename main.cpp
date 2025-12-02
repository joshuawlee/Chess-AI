#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Renderer.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(Renderer::BOARD_SIZE), 
                                           static_cast<unsigned int>(Renderer::BOARD_SIZE)}), 
                           "C++ Chess MVP");
    window.setFramerateLimit(60);
    
    Game game;
    Renderer renderer;
    
    if(!renderer.loadAssets()) {
        std::cerr << "Failed to load assets. Please ensure piece images are in assets/ folder." << std::endl;
        std::cerr << "Required files:" << std::endl;
        std::cerr << "  assets/white_pawn.png" << std::endl;
        std::cerr << "  assets/white_knight.png" << std::endl;
        std::cerr << "  assets/white_bishop.png" << std::endl;
        std::cerr << "  assets/white_rook.png" << std::endl;
        std::cerr << "  assets/white_queen.png" << std::endl;
        std::cerr << "  assets/white_king.png" << std::endl;
        std::cerr << "  assets/black_pawn.png" << std::endl;
        std::cerr << "  assets/black_knight.png" << std::endl;
        std::cerr << "  assets/black_bishop.png" << std::endl;
        std::cerr << "  assets/black_rook.png" << std::endl;
        std::cerr << "  assets/black_queen.png" << std::endl;
        std::cerr << "  assets/black_king.png" << std::endl;
        return -1;
    }
    
    int selectedRow = -1;
    int selectedCol = -1;
    bool pieceSelected = false;
    GameMode currentMode = GameMode::MainMenu;
    
    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if(game.getMode() == GameMode::MainMenu) {
                    if(keyPressed->code == sf::Keyboard::Key::Num1) {
                        currentMode = GameMode::PVP;
                        game = Game(); // Reset game
                    } else if(keyPressed->code == sf::Keyboard::Key::Num2) {
                        currentMode = GameMode::PVAI;
                        game = Game(); // Reset game
                    }
                } else if(game.getMode() == GameMode::GameOver) {
                    if(keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentMode = GameMode::MainMenu;
                        game = Game(); // Reset game
                        pieceSelected = false;
                        selectedRow = -1;
                        selectedCol = -1;
                    }
                }
            }
            
            if(const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if(mousePressed->button == sf::Mouse::Button::Left) {
                    if(game.getMode() == GameMode::PVP || game.getMode() == GameMode::PVAI) {
                        int col = mousePressed->position.x / Renderer::TILE_SIZE;
                        int row = mousePressed->position.y / Renderer::TILE_SIZE;
                        
                        if(row >= 0 && row < 8 && col >= 0 && col < 8) {
                            if(!pieceSelected) {
                                // First click - select piece
                                Piece piece = game.getBoard().getPiece(row, col);
                                PieceColor currentColor = game.getBoard().whiteToMove ? PieceColor::White : PieceColor::Black;
                                
                                // In PVAI mode, only allow white pieces to be selected
                                if(game.getMode() == GameMode::PVAI && currentColor == PieceColor::Black) {
                                    continue;
                                }
                                
                                if(piece.type != PieceType::None && piece.color == currentColor) {
                                    selectedRow = row;
                                    selectedCol = col;
                                    pieceSelected = true;
                                }
                            } else {
                                // Second click - try to move
                                Move attemptedMove;
                                attemptedMove.startRow = selectedRow;
                                attemptedMove.startCol = selectedCol;
                                attemptedMove.endRow = row;
                                attemptedMove.endCol = col;
                                attemptedMove.movedPiece = game.getBoard().getPiece(selectedRow, selectedCol);
                                attemptedMove.capturedPiece = game.getBoard().getPiece(row, col);
                                
                                // Check if this move is legal
                                std::vector<Move> legalMoves = game.getBoard().generateLegalMoves();
                                bool isLegal = false;
                                Move legalMove;
                                
                                for(const Move& move : legalMoves) {
                                    if(move.startRow == attemptedMove.startRow &&
                                       move.startCol == attemptedMove.startCol &&
                                       move.endRow == attemptedMove.endRow &&
                                       move.endCol == attemptedMove.endCol) {
                                        isLegal = true;
                                        legalMove = move;
                                        break;
                                    }
                                }
                                
                                if(isLegal) {
                                    game.getBoard().makeMove(legalMove);
                                    
                                    // Check for game end
                                    if(game.getBoard().isCheckmate()) {
                                        currentMode = GameMode::GameOver;
                                    } else if(game.getBoard().isStalemate()) {
                                        currentMode = GameMode::GameOver;
                                    }
                                }
                                
                                pieceSelected = false;
                                selectedRow = -1;
                                selectedCol = -1;
                            }
                        }
                    }
                }
            }
        }
        
        // AI move in PVAI mode
        if(currentMode == GameMode::PVAI && !game.getBoard().whiteToMove && !game.isGameOver()) {
            AI ai(3);
            Move aiMove = ai.getBestMove(game.getBoard());
            if(aiMove.movedPiece.type != PieceType::None) {
                game.getBoard().makeMove(aiMove);
                
                // Check for game end after AI move
                if(game.getBoard().isCheckmate()) {
                    currentMode = GameMode::GameOver;
                } else if(game.getBoard().isStalemate()) {
                    currentMode = GameMode::GameOver;
                }
            }
        }
        
        // Update game mode for rendering
        Game tempGame = game;
        if(currentMode == GameMode::MainMenu) {
            game = Game();
        }
        
        // Render
        renderer.render(window, game, selectedRow, selectedCol, pieceSelected);
        
        // Set the actual game mode for display
        if(currentMode != game.getMode()) {
            // This handles mode transitions for rendering
        }
    }
    
    return 0;
}