#include "Game.h"
#include <iostream> // For debugging

Game::Game() {
    mode = GameMode::MainMenu;
    previousMode = GameMode::MainMenu;
    gameOver = false;
    selectedRow = -1;
    selectedCol = -1;
    pieceSelected = false;
}

void Game::run() {
    // SFML 3: VideoMode takes Vector2u
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 800)), "Chess Game");
    window.setFramerateLimit(60);
    
    if (!renderer.loadAssets()) {
        return; // Failed to load assets
    }
    
    while (window.isOpen()) {
        handleInput(window);
        update();
        render(window);
    }
}

void Game::handleInput(sf::RenderWindow& window) {
    // SFML 3: pollEvent() returns optional<Event>, no parameter
    while (std::optional<sf::Event> event = window.pollEvent()) {
        // SFML 3: Use std::holds_alternative to check event type
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        // Handle ESC key to pause/unpause - DEBUGGING VERSION
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            std::cout << "Key pressed detected! Code: " << static_cast<int>(keyPressed->code) << std::endl;
            std::cout << "Escape code: " << static_cast<int>(sf::Keyboard::Key::Escape) << std::endl;
            
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                std::cout << "ESC detected! Current mode: " << static_cast<int>(mode) << std::endl;
                if (mode == GameMode::PVP || mode == GameMode::PVAI) {
                    // Pause the game
                    std::cout << "Pausing game..." << std::endl;
                    previousMode = mode;
                    mode = GameMode::Paused;
                } else if (mode == GameMode::Paused) {
                    // Unpause the game
                    std::cout << "Unpausing game..." << std::endl;
                    mode = previousMode;
                }
            }
        }
        
        // Alternative: Try KeyReleased instead of KeyPressed
        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            std::cout << "Key released detected! Code: " << static_cast<int>(keyReleased->code) << std::endl;
            
            if (keyReleased->code == sf::Keyboard::Key::Escape) {
                std::cout << "ESC released! Current mode: " << static_cast<int>(mode) << std::endl;
                if (mode == GameMode::PVP || mode == GameMode::PVAI) {
                    std::cout << "Pausing game (on release)..." << std::endl;
                    previousMode = mode;
                    mode = GameMode::Paused;
                } else if (mode == GameMode::Paused) {
                    std::cout << "Unpausing game (on release)..." << std::endl;
                    mode = previousMode;
                }
            }
        }
        
        // SFML 3: Check for MouseButtonPressed event
        if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
            int mouseX = mouseButton->position.x;
            int mouseY = mouseButton->position.y;
            
            if (mode == GameMode::MainMenu) {
                // Check PVP button (250, 350, 300x60)
                if (mouseX >= 250 && mouseX <= 550 && mouseY >= 350 && mouseY <= 410) {
                    selectGameMode(GameMode::PVP);
                }
                // Check PVAI button (250, 450, 300x60)
                else if (mouseX >= 250 && mouseX <= 550 && mouseY >= 450 && mouseY <= 510) {
                    selectGameMode(GameMode::PVAI);
                }
            }
            else if (mode == GameMode::Paused) {
                // Check Resume button (250, 300, 300x60)
                if (mouseX >= 250 && mouseX <= 550 && mouseY >= 300 && mouseY <= 360) {
                    mode = previousMode;
                }
                // Check Return to Menu button (250, 400, 300x60)
                else if (mouseX >= 250 && mouseX <= 550 && mouseY >= 400 && mouseY <= 460) {
                    mode = GameMode::MainMenu;
                    resetGame();
                }
            }
            else if (mode == GameMode::GameOver) {
                // Check return button (250, 450, 300x60)
                if (mouseX >= 250 && mouseX <= 550 && mouseY >= 450 && mouseY <= 510) {
                    mode = GameMode::MainMenu;
                }
            }
            else if (mode == GameMode::PVP || mode == GameMode::PVAI) {
                int col = mouseX / Renderer::TILE_SIZE;
                int row = mouseY / Renderer::TILE_SIZE;
                
                if (row >= 0 && row < 8 && col >= 0 && col < 8) {
                    handleBoardClick(row, col);
                }
            }
        }
    }
}

void Game::update() {
    // Don't update game state when paused
    if (mode == GameMode::Paused) {
        return;
    }
    
    if (mode == GameMode::PVAI && !gameOver && !board.whiteToMove) {
        handleAITurn();
    }
    
    if ((mode == GameMode::PVP || mode == GameMode::PVAI) && !gameOver) {
        checkGameEndState();
    }
}

void Game::render(sf::RenderWindow& window) {
    window.clear();
    
    if (mode == GameMode::MainMenu) {
        renderer.renderMainMenu(window);
    }
    else if (mode == GameMode::Paused) {
        // Draw the game board in background
        renderer.renderGame(window, board, selectedRow, selectedCol, pieceSelected);
        // Draw pause menu overlay
        renderer.renderPauseMenu(window);
    }
    else if (mode == GameMode::GameOver) {
        renderer.renderGameOver(window, resultText);
    }
    else {
        renderer.renderGame(window, board, selectedRow, selectedCol, pieceSelected);
    }
    
    window.display();
}

void Game::resetGame() {
    board = Board();
    gameOver = false;
    selectedRow = -1;
    selectedCol = -1;
    pieceSelected = false;
    resultText = "";
}

void Game::selectGameMode(GameMode newMode) {
    mode = newMode;
    resetGame();
}

void Game::handleBoardClick(int row, int col) {
    if (gameOver) return;
    
    // PVAI mode: only allow white to move
    if (mode == GameMode::PVAI && !board.whiteToMove) {
        return;
    }
    
    if (!pieceSelected) {
        // Select a piece
        Piece piece = board.getPiece(row, col);
        PieceColor currentColor = board.whiteToMove ? PieceColor::White : PieceColor::Black;
        
        if (!piece.isEmpty() && piece.color == currentColor) {
            selectedRow = row;
            selectedCol = col;
            pieceSelected = true;
        }
    }
    else {
        // Try to move the selected piece
        Move attemptedMove(selectedRow, selectedCol, row, col);
        attemptedMove.movedPiece = board.getPiece(selectedRow, selectedCol);
        attemptedMove.capturedPiece = board.getPiece(row, col);
        
        // Check if this move is legal
        std::vector<Move> legalMoves = board.generateLegalMoves();
        bool isLegal = false;
        Move legalMove;
        
        for (const Move& move : legalMoves) {
            if (move.startRow == selectedRow && move.startCol == selectedCol &&
                move.endRow == row && move.endCol == col) {
                isLegal = true;
                legalMove = move;
                break;
            }
        }
        
        if (isLegal) {
            board.makeMove(legalMove);
        }
        
        // Deselect
        pieceSelected = false;
        selectedRow = -1;
        selectedCol = -1;
    }
}

void Game::checkGameEndState() {
    if (!board.hasLegalMoves()) {
        gameOver = true;
        PieceColor currentColor = board.whiteToMove ? PieceColor::White : PieceColor::Black;
        
        if (board.inCheck(currentColor)) {
            // Checkmate
            if (board.whiteToMove) {
                resultText = "Black Wins!";
            } else {
                resultText = "White Wins!";
            }
        } else {
            // Stalemate
            resultText = "Draw - Stalemate";
        }
        
        mode = GameMode::GameOver;
    }
}

void Game::handleAITurn() {
    Move aiMove = ai.getBestMove(board, 3);
    
    if (aiMove.startRow >= 0 && aiMove.startCol >= 0) {
        board.makeMove(aiMove);
    }
}