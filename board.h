#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Move.h"
#include <vector>

class Board {
public:
    Piece squares[8][8];
    bool whiteToMove;
    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteRookKingsideMoved;
    bool whiteRookQueensideMoved;
    bool blackRookKingsideMoved;
    bool blackRookQueensideMoved;
    int enPassantCol;
    int enPassantRow;

    Board();
    void initializeBoard();
    std::vector<Move> generateLegalMoves();
    std::vector<Move> generatePseudoLegalMoves();
    bool isSquareAttacked(int row, int col, PieceColor attacker);
    bool makeMove(const Move& move);
    void undoMove(const Move& move);
    bool isInCheck(PieceColor color);
    bool isCheckmate();
    bool isStalemate();
    Piece getPiece(int row, int col) const;
    void setPiece(int row, int col, Piece piece);

private:
    void generatePawnMoves(int row, int col, std::vector<Move>& moves);
    void generateKnightMoves(int row, int col, std::vector<Move>& moves);
    void generateBishopMoves(int row, int col, std::vector<Move>& moves);
    void generateRookMoves(int row, int col, std::vector<Move>& moves);
    void generateQueenMoves(int row, int col, std::vector<Move>& moves);
    void generateKingMoves(int row, int col, std::vector<Move>& moves);
    void generateSlidingMoves(int row, int col, std::vector<Move>& moves, 
                            const std::vector<std::pair<int,int>>& directions);
    bool isValidSquare(int row, int col) const;
};

#endif