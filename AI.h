#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Move.h"
#include <limits>

class AI {
public:
    AI(int depth = 3);
    Move getBestMove(Board& board);

private:
    int depth;
    int evaluateBoard(const Board& board);
    int pieceValue(PieceType type);
    int minimax(Board& board, int depth, int alpha, int beta, bool maximizingPlayer);
};

#endif