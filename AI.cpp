#include "AI.h"

AI::AI(int depth) : depth(depth) {}

int AI::pieceValue(PieceType type) {
    switch(type) {
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
    
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            Piece piece = board.squares[row][col];
            if(piece.type != PieceType::None) {
                int value = pieceValue(piece.type);
                if(piece.color == PieceColor::White) {
                    score += value;
                } else {
                    score -= value;
                }
            }
        }
    }
    
    return score;
}

int AI::minimax(Board& board, int depth, int alpha, int beta, bool maximizingPlayer) {
    if(depth == 0 || board.isCheckmate() || board.isStalemate()) {
        return evaluateBoard(board);
    }

    std::vector<Move> legalMoves = board.generateLegalMoves();

    if(maximizingPlayer) {
        int maxEval = std::numeric_limits<int>::min();
        for(const Move& move : legalMoves) {
            // Save state
            bool savedWhiteKingMoved = board.whiteKingMoved;
            bool savedBlackKingMoved = board.blackKingMoved;
            bool savedWhiteRookKMoved = board.whiteRookKingsideMoved;
            bool savedWhiteRookQMoved = board.whiteRookQueensideMoved;
            bool savedBlackRookKMoved = board.blackRookKingsideMoved;
            bool savedBlackRookQMoved = board.blackRookQueensideMoved;
            int savedEnPassantCol = board.enPassantCol;
            int savedEnPassantRow = board.enPassantRow;

            board.makeMove(move);
            int eval = minimax(board, depth - 1, alpha, beta, false);
            board.undoMove(move);

            // Restore state
            board.whiteKingMoved = savedWhiteKingMoved;
            board.blackKingMoved = savedBlackKingMoved;
            board.whiteRookKingsideMoved = savedWhiteRookKMoved;
            board.whiteRookQueensideMoved = savedWhiteRookQMoved;
            board.blackRookKingsideMoved = savedBlackRookKMoved;
            board.blackRookQueensideMoved = savedBlackRookQMoved;
            board.enPassantCol = savedEnPassantCol;
            board.enPassantRow = savedEnPassantRow;

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if(beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for(const Move& move : legalMoves) {
            // Save state
            bool savedWhiteKingMoved = board.whiteKingMoved;
            bool savedBlackKingMoved = board.blackKingMoved;
            bool savedWhiteRookKMoved = board.whiteRookKingsideMoved;
            bool savedWhiteRookQMoved = board.whiteRookQueensideMoved;
            bool savedBlackRookKMoved = board.blackRookKingsideMoved;
            bool savedBlackRookQMoved = board.blackRookQueensideMoved;
            int savedEnPassantCol = board.enPassantCol;
            int savedEnPassantRow = board.enPassantRow;

            board.makeMove(move);
            int eval = minimax(board, depth - 1, alpha, beta, true);
            board.undoMove(move);

            // Restore state
            board.whiteKingMoved = savedWhiteKingMoved;
            board.blackKingMoved = savedBlackKingMoved;
            board.whiteRookKingsideMoved = savedWhiteRookKMoved;
            board.whiteRookQueensideMoved = savedWhiteRookQMoved;
            board.blackRookKingsideMoved = savedBlackRookKMoved;
            board.blackRookQueensideMoved = savedBlackRookQMoved;
            board.enPassantCol = savedEnPassantCol;
            board.enPassantRow = savedEnPassantRow;

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if(beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

Move AI::getBestMove(Board& board) {
    std::vector<Move> legalMoves = board.generateLegalMoves();
    if(legalMoves.empty()) {
        return Move();
    }

    Move bestMove = legalMoves[0];
    int bestScore = std::numeric_limits<int>::min();

    for(const Move& move : legalMoves) {
        // Save state
        bool savedWhiteKingMoved = board.whiteKingMoved;
        bool savedBlackKingMoved = board.blackKingMoved;
        bool savedWhiteRookKMoved = board.whiteRookKingsideMoved;
        bool savedWhiteRookQMoved = board.whiteRookQueensideMoved;
        bool savedBlackRookKMoved = board.blackRookKingsideMoved;
        bool savedBlackRookQMoved = board.blackRookQueensideMoved;
        int savedEnPassantCol = board.enPassantCol;
        int savedEnPassantRow = board.enPassantRow;

        board.makeMove(move);
        int score = minimax(board, depth - 1, std::numeric_limits<int>::min(), 
                           std::numeric_limits<int>::max(), true);
        board.undoMove(move);

        // Restore state
        board.whiteKingMoved = savedWhiteKingMoved;
        board.blackKingMoved = savedBlackKingMoved;
        board.whiteRookKingsideMoved = savedWhiteRookKMoved;
        board.whiteRookQueensideMoved = savedWhiteRookQMoved;
        board.blackRookKingsideMoved = savedBlackRookKMoved;
        board.blackRookQueensideMoved = savedBlackRookQMoved;
        board.enPassantCol = savedEnPassantCol;
        board.enPassantRow = savedEnPassantRow;

        if(score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}