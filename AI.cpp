#include "AI.h"
#include <limits>

AI::AI() {}

int AI::getPieceValue(PieceType type) {
    switch (type) {
        case PieceType::Pawn: return 100;
        case PieceType::Knight: return 300;
        case PieceType::Bishop: return 300;
        case PieceType::Rook: return 500;
        case PieceType::Queen: return 900;
        case PieceType::King: return 10000;
        default: return 0;
    }
}

int AI::evaluateBoard(const Board& board) {
    int score = 0;
    
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece piece = board.squares[r][c];
            if (piece.isEmpty()) continue;
            
            int value = getPieceValue(piece.type);
            
            if (piece.color == PieceColor::White) {
                score += value;
            } else {
                score -= value;
            }
        }
    }
    
    return score;
}

int AI::minimax(Board& board, int depth, int alpha, int beta, bool maximizing) {
    if (depth == 0) {
        return evaluateBoard(board);
    }
    
    std::vector<Move> moves = board.generateLegalMoves();
    
    if (moves.empty()) {
        PieceColor currentColor = board.whiteToMove ? PieceColor::White : PieceColor::Black;
        if (board.inCheck(currentColor)) {
            // Checkmate
            return maximizing ? -100000 : 100000;
        } else {
            // Stalemate
            return 0;
        }
    }
    
    if (maximizing) {
        int maxEval = std::numeric_limits<int>::min();
        for (const Move& move : moves) {
            board.makeMove(move);
            int eval = minimax(board, depth - 1, alpha, beta, false);
            board.undoMove(move);
            
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // Beta cutoff
            }
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const Move& move : moves) {
            board.makeMove(move);
            int eval = minimax(board, depth - 1, alpha, beta, true);
            board.undoMove(move);
            
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // Alpha cutoff
            }
        }
        return minEval;
    }
}

Move AI::getBestMove(Board& board, int depth) {
    std::vector<Move> moves = board.generateLegalMoves();
    
    if (moves.empty()) {
        return Move();
    }
    
    Move bestMove = moves[0];
    int bestScore = std::numeric_limits<int>::max();
    
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();
    
    for (const Move& move : moves) {
        board.makeMove(move);
        int score = minimax(board, depth - 1, alpha, beta, true);
        board.undoMove(move);
        
        if (score < bestScore) {
            bestScore = score;
            bestMove = move;
        }
        
        beta = std::min(beta, score);
    }
    
    return bestMove;
}