#include "Game.h"

Game::Game() : ai(3), mode(GameMode::MainMenu), running(true), 
               gameOver(false), selectedRow(-1), selectedCol(-1), 
               pieceSelected(false) {}

void Game::run() {
    // Game logic handled by main loop in main.cpp
}

bool Game::isRunning() const {
    return running;
}

Board& Game::getBoard() {
    return board;
}

GameMode Game::getMode() const {
    return mode;
}

void Game::selectMode(GameMode newMode) {
    mode = newMode;
    board = Board(); // Reset board
    gameOver = false;
    gameResult = "";
    pieceSelected = false;
    selectedRow = -1;
    selectedCol = -1;
}

void Game::handlePieceSelection(int row, int col) {
    if(gameOver) return;

    if(!pieceSelected) {
        // First click - select piece
        Piece piece = board.getPiece(row, col);
        PieceColor currentColor = board.whiteToMove ? PieceColor::White : PieceColor::Black;
        
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
        attemptedMove.movedPiece = board.getPiece(selectedRow, selectedCol);
        attemptedMove.capturedPiece = board.getPiece(row, col);
        attemptedMove.isPromotion = false;
        attemptedMove.isCastle = false;
        attemptedMove.isEnPassant = false;

        // Check if this move is legal
        std::vector<Move> legalMoves = board.generateLegalMoves();
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
            board.makeMove(legalMove);
            checkGameEnd();
        }

        pieceSelected = false;
        selectedRow = -1;
        selectedCol = -1;
    }
}

void Game::update() {
    if(mode == GameMode::PVAI && !board.whiteToMove && !gameOver) {
        // AI's turn (playing as Black)
        Move aiMove = ai.getBestMove(board);
        if(aiMove.movedPiece.type != PieceType::None) {
            board.makeMove(aiMove);
            checkGameEnd();
        }
    }
}

void Game::checkGameEnd() {
    if(board.isCheckmate()) {
        gameOver = true;
        gameResult = board.whiteToMove ? "Black wins by checkmate!" : "White wins by checkmate!";
        mode = GameMode::GameOver;
    } else if(board.isStalemate()) {
        gameOver = true;
        gameResult = "Draw by stalemate!";
        mode = GameMode::GameOver;
    }
}

bool Game::isGameOver() const {
    return gameOver;
}

std::string Game::getGameResult() const {
    return gameResult;
}

void Game::handleInput() {
    // Handled in main.cpp with SFML events
}