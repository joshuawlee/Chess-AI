#ifndef MOVE_H
#define MOVE_H

#include "Piece.h"

struct Move {
    int startRow, startCol;
    int endRow, endCol;
    
    Piece movedPiece;
    Piece capturedPiece;
    
    bool isPromotion = false;
    PieceType promotionType = PieceType::Queen;
    
    bool isCastle = false;
    bool isEnPassant = false;
    
    Move() : startRow(0), startCol(0), endRow(0), endCol(0) {}
    
    Move(int sr, int sc, int er, int ec) 
        : startRow(sr), startCol(sc), endRow(er), endCol(ec) {}
};

#endif // MOVE_H