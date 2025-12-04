#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Move.h"

class AI {
public:
    AI();
    Move getBestMove(Board& board, int depth = 3);
    
private:
    int evaluateBoard(const Board& board);
    int minimax(Board& board, int depth, int alpha, int beta, bool maximizing);
    int getPieceValue(PieceType type);
};

#endif // AI_H