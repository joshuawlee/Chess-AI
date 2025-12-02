#ifndef MOVE_H
#define MOVE_H

#include "Piece.h"

struct Move {
    int startRow;
    int startCol;
    int endRow;
    int endCol;
    Piece capturedPiece;
    Piece movedPiece;
    bool isPromotion;
    PieceType promotionType;
    bool isCastle;
    bool isEnPassant;

    Move() : startRow(0), startCol(0), endRow(0), endCol(0),
             capturedPiece({PieceType::None, PieceColor::NoColor}),
             movedPiece({PieceType::None, PieceColor::NoColor}),
             isPromotion(false), promotionType(PieceType::Queen),
             isCastle(false), isEnPassant(false) {}
};

#endif