#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Move.h"
#include <vector>

class Board {
public:
    Piece squares[8][8];
    bool whiteToMove;
    
    int enPassantCol;
    int enPassantRow;
    
    bool whiteKingMoved;
    bool whiteRookKingsideMoved;
    bool whiteRookQueensideMoved;
    bool blackKingMoved;
    bool blackRookKingsideMoved;
    bool blackRookQueensideMoved;
    
    Board();
    
    std::vector<Move> generateLegalMoves();
    bool makeMove(const Move& move);
    void undoMove(const Move& move);
    bool isSquareAttacked(int row, int col, PieceColor attacker);
    bool inCheck(PieceColor color);
    bool hasLegalMoves();
    
    Piece getPiece(int row, int col) const;
    void setPiece(int row, int col, Piece piece);
    
private:
    std::vector<Move> generatePseudoLegalMoves();
    void generatePawnMoves(int row, int col, std::vector<Move>& moves);
    void generateKnightMoves(int row, int col, std::vector<Move>& moves);
    void generateBishopMoves(int row, int col, std::vector<Move>& moves);
    void generateRookMoves(int row, int col, std::vector<Move>& moves);
    void generateQueenMoves(int row, int col, std::vector<Move>& moves);
    void generateKingMoves(int row, int col, std::vector<Move>& moves);
    
    bool isValidSquare(int row, int col) const;
    void findKing(PieceColor color, int& kingRow, int& kingCol);
};

#endif // BOARD_H