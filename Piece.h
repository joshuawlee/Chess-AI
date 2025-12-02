#ifndef PIECE_H
#define PIECE_H

enum class PieceType {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    None
};

enum class PieceColor {
    White,
    Black,
    NoColor
};

struct Piece {
    PieceType type;
    PieceColor color;
};

#endif