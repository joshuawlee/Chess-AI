#ifndef PIECE_H
#define PIECE_H

enum class PieceType {
    Pawn, Knight, Bishop, Rook, Queen, King, None
};

enum class PieceColor {
    White, Black, None
};

struct Piece {
    PieceType type;
    PieceColor color;
    
    Piece() : type(PieceType::None), color(PieceColor::None) {}
    Piece(PieceType t, PieceColor c) : type(t), color(c) {}
    
    bool isEmpty() const {
        return type == PieceType::None;
    }
};

#endif // PIECE_H