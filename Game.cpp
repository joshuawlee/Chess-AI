#include "Game.h"

Game::Game() {
    mode = GameMode::MainMenu;
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