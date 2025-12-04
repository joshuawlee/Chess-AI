#include "Board.h"

Board::Board() {
    whiteToMove = true;
    enPassantCol = -1;
    enPassantRow = -1;
    
    whiteKingMoved = false;
    whiteRookKingsideMoved = false;
    whiteRookQueensideMoved = false;
    blackKingMoved = false;
    blackRookKingsideMoved = false;
    blackRookQueensideMoved = false;
    
    // Initialize empty board
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            squares[r][c] = Piece(PieceType::None, PieceColor::None);
        }
    }
    
    // Set up initial position
    // Black pieces (top)
    squares[0][0] = Piece(PieceType::Rook, PieceColor::Black);
    squares[0][1] = Piece(PieceType::Knight, PieceColor::Black);
    squares[0][2] = Piece(PieceType::Bishop, PieceColor::Black);
    squares[0][3] = Piece(PieceType::Queen, PieceColor::Black);
    squares[0][4] = Piece(PieceType::King, PieceColor::Black);
    squares[0][5] = Piece(PieceType::Bishop, PieceColor::Black);
    squares[0][6] = Piece(PieceType::Knight, PieceColor::Black);
    squares[0][7] = Piece(PieceType::Rook, PieceColor::Black);
    
    for (int c = 0; c < 8; c++) {
        squares[1][c] = Piece(PieceType::Pawn, PieceColor::Black);
    }
    
    // White pieces (bottom)
    for (int c = 0; c < 8; c++) {
        squares[6][c] = Piece(PieceType::Pawn, PieceColor::White);
    }
    
    squares[7][0] = Piece(PieceType::Rook, PieceColor::White);
    squares[7][1] = Piece(PieceType::Knight, PieceColor::White);
    squares[7][2] = Piece(PieceType::Bishop, PieceColor::White);
    squares[7][3] = Piece(PieceType::Queen, PieceColor::White);
    squares[7][4] = Piece(PieceType::King, PieceColor::White);
    squares[7][5] = Piece(PieceType::Bishop, PieceColor::White);
    squares[7][6] = Piece(PieceType::Knight, PieceColor::White);
    squares[7][7] = Piece(PieceType::Rook, PieceColor::White);
}

Piece Board::getPiece(int row, int col) const {
    if (isValidSquare(row, col)) {
        return squares[row][col];
    }
    return Piece();
}

void Board::setPiece(int row, int col, Piece piece) {
    if (isValidSquare(row, col)) {
        squares[row][col] = piece;
    }
}

bool Board::isValidSquare(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

std::vector<Move> Board::generateLegalMoves() {
    std::vector<Move> pseudoLegal = generatePseudoLegalMoves();
    std::vector<Move> legal;
    
    for (const Move& move : pseudoLegal) {
        // Make the move
        Piece temp = squares[move.endRow][move.endCol];
        squares[move.endRow][move.endCol] = squares[move.startRow][move.startCol];
        squares[move.startRow][move.startCol] = Piece();
        
        // Check if own king is in check
        PieceColor currentColor = whiteToMove ? PieceColor::White : PieceColor::Black;
        bool safe = !inCheck(currentColor);
        
        // Undo the move
        squares[move.startRow][move.startCol] = squares[move.endRow][move.endCol];
        squares[move.endRow][move.endCol] = temp;
        
        if (safe) {
            legal.push_back(move);
        }
    }
    
    return legal;
}

std::vector<Move> Board::generatePseudoLegalMoves() {
    std::vector<Move> moves;
    PieceColor currentColor = whiteToMove ? PieceColor::White : PieceColor::Black;
    
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece piece = squares[r][c];
            if (piece.isEmpty() || piece.color != currentColor) {
                continue;
            }
            
            switch (piece.type) {
                case PieceType::Pawn:
                    generatePawnMoves(r, c, moves);
                    break;
                case PieceType::Knight:
                    generateKnightMoves(r, c, moves);
                    break;
                case PieceType::Bishop:
                    generateBishopMoves(r, c, moves);
                    break;
                case PieceType::Rook:
                    generateRookMoves(r, c, moves);
                    break;
                case PieceType::Queen:
                    generateQueenMoves(r, c, moves);
                    break;
                case PieceType::King:
                    generateKingMoves(r, c, moves);
                    break;
                default:
                    break;
            }
        }
    }
    
    return moves;
}

void Board::generatePawnMoves(int row, int col, std::vector<Move>& moves) {
    Piece pawn = squares[row][col];
    int direction = (pawn.color == PieceColor::White) ? -1 : 1;
    int startRow = (pawn.color == PieceColor::White) ? 6 : 1;
    int promotionRow = (pawn.color == PieceColor::White) ? 0 : 7;
    
    // Forward move
    int newRow = row + direction;
    if (isValidSquare(newRow, col) && squares[newRow][col].isEmpty()) {
        Move move(row, col, newRow, col);
        move.movedPiece = pawn;
        
        if (newRow == promotionRow) {
            move.isPromotion = true;
        }
        moves.push_back(move);
        
        // Double move from start
        if (row == startRow) {
            int doubleRow = row + 2 * direction;
            if (squares[doubleRow][col].isEmpty()) {
                Move doubleMove(row, col, doubleRow, col);
                doubleMove.movedPiece = pawn;
                moves.push_back(doubleMove);
            }
        }
    }
    
    // Captures
    for (int dc = -1; dc <= 1; dc += 2) {
        int newCol = col + dc;
        if (isValidSquare(newRow, newCol)) {
            Piece target = squares[newRow][newCol];
            if (!target.isEmpty() && target.color != pawn.color) {
                Move move(row, col, newRow, newCol);
                move.movedPiece = pawn;
                move.capturedPiece = target;
                
                if (newRow == promotionRow) {
                    move.isPromotion = true;
                }
                moves.push_back(move);
            }
            
            // En passant
            if (newRow == enPassantRow && newCol == enPassantCol) {
                Move move(row, col, newRow, newCol);
                move.movedPiece = pawn;
                move.capturedPiece = squares[row][newCol];
                move.isEnPassant = true;
                moves.push_back(move);
            }
        }
    }
}

void Board::generateKnightMoves(int row, int col, std::vector<Move>& moves) {
    Piece knight = squares[row][col];
    int offsets[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    
    for (int i = 0; i < 8; i++) {
        int newRow = row + offsets[i][0];
        int newCol = col + offsets[i][1];
        
        if (isValidSquare(newRow, newCol)) {
            Piece target = squares[newRow][newCol];
            if (target.isEmpty() || target.color != knight.color) {
                Move move(row, col, newRow, newCol);
                move.movedPiece = knight;
                move.capturedPiece = target;
                moves.push_back(move);
            }
        }
    }
}

void Board::generateBishopMoves(int row, int col, std::vector<Move>& moves) {
    Piece bishop = squares[row][col];
    int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    
    for (int d = 0; d < 4; d++) {
        int dr = directions[d][0];
        int dc = directions[d][1];
        
        for (int dist = 1; dist < 8; dist++) {
            int newRow = row + dr * dist;
            int newCol = col + dc * dist;
            
            if (!isValidSquare(newRow, newCol)) break;
            
            Piece target = squares[newRow][newCol];
            if (target.isEmpty()) {
                Move move(row, col, newRow, newCol);
                move.movedPiece = bishop;
                moves.push_back(move);
            } else {
                if (target.color != bishop.color) {
                    Move move(row, col, newRow, newCol);
                    move.movedPiece = bishop;
                    move.capturedPiece = target;
                    moves.push_back(move);
                }
                break;
            }
        }
    }
}

void Board::generateRookMoves(int row, int col, std::vector<Move>& moves) {
    Piece rook = squares[row][col];
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    for (int d = 0; d < 4; d++) {
        int dr = directions[d][0];
        int dc = directions[d][1];
        
        for (int dist = 1; dist < 8; dist++) {
            int newRow = row + dr * dist;
            int newCol = col + dc * dist;
            
            if (!isValidSquare(newRow, newCol)) break;
            
            Piece target = squares[newRow][newCol];
            if (target.isEmpty()) {
                Move move(row, col, newRow, newCol);
                move.movedPiece = rook;
                moves.push_back(move);
            } else {
                if (target.color != rook.color) {
                    Move move(row, col, newRow, newCol);
                    move.movedPiece = rook;
                    move.capturedPiece = target;
                    moves.push_back(move);
                }
                break;
            }
        }
    }
}

void Board::generateQueenMoves(int row, int col, std::vector<Move>& moves) {
    generateBishopMoves(row, col, moves);
    generateRookMoves(row, col, moves);
}

void Board::generateKingMoves(int row, int col, std::vector<Move>& moves) {
    Piece king = squares[row][col];
    int offsets[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
        {0, 1}, {1, -1}, {1, 0}, {1, 1}
    };
    
    for (int i = 0; i < 8; i++) {
        int newRow = row + offsets[i][0];
        int newCol = col + offsets[i][1];
        
        if (isValidSquare(newRow, newCol)) {
            Piece target = squares[newRow][newCol];
            if (target.isEmpty() || target.color != king.color) {
                Move move(row, col, newRow, newCol);
                move.movedPiece = king;
                move.capturedPiece = target;
                moves.push_back(move);
            }
        }
    }
    
    // Castling
    bool isWhite = king.color == PieceColor::White;
    bool kingMoved = isWhite ? whiteKingMoved : blackKingMoved;
    
    if (!kingMoved && !inCheck(king.color)) {
        int homeRow = isWhite ? 7 : 0;
        
        // Kingside
        bool kingsideRookMoved = isWhite ? whiteRookKingsideMoved : blackRookKingsideMoved;
        if (!kingsideRookMoved && 
            squares[homeRow][5].isEmpty() && 
            squares[homeRow][6].isEmpty() &&
            !isSquareAttacked(homeRow, 5, isWhite ? PieceColor::Black : PieceColor::White) &&
            !isSquareAttacked(homeRow, 6, isWhite ? PieceColor::Black : PieceColor::White)) {
            
            Move castle(row, col, homeRow, 6);
            castle.movedPiece = king;
            castle.isCastle = true;
            moves.push_back(castle);
        }
        
        // Queenside
        bool queensideRookMoved = isWhite ? whiteRookQueensideMoved : blackRookQueensideMoved;
        if (!queensideRookMoved && 
            squares[homeRow][1].isEmpty() && 
            squares[homeRow][2].isEmpty() && 
            squares[homeRow][3].isEmpty() &&
            !isSquareAttacked(homeRow, 2, isWhite ? PieceColor::Black : PieceColor::White) &&
            !isSquareAttacked(homeRow, 3, isWhite ? PieceColor::Black : PieceColor::White)) {
            
            Move castle(row, col, homeRow, 2);
            castle.movedPiece = king;
            castle.isCastle = true;
            moves.push_back(castle);
        }
    }
}

bool Board::makeMove(const Move& move) {
    // Reset en passant
    enPassantCol = -1;
    enPassantRow = -1;
    
    Piece piece = squares[move.startRow][move.startCol];
    
    // Handle castling
    if (move.isCastle) {
        squares[move.endRow][move.endCol] = piece;
        squares[move.startRow][move.startCol] = Piece();
        
        // Move rook
        if (move.endCol == 6) { // Kingside
            Piece rook = squares[move.endRow][7];
            squares[move.endRow][5] = rook;
            squares[move.endRow][7] = Piece();
        } else { // Queenside
            Piece rook = squares[move.endRow][0];
            squares[move.endRow][3] = rook;
            squares[move.endRow][0] = Piece();
        }
    }
    // Handle en passant
    else if (move.isEnPassant) {
        squares[move.endRow][move.endCol] = piece;
        squares[move.startRow][move.startCol] = Piece();
        squares[move.startRow][move.endCol] = Piece(); // Remove captured pawn
    }
    // Handle promotion
    else if (move.isPromotion) {
        squares[move.endRow][move.endCol] = Piece(move.promotionType, piece.color);
        squares[move.startRow][move.startCol] = Piece();
    }
    // Normal move
    else {
        squares[move.endRow][move.endCol] = piece;
        squares[move.startRow][move.startCol] = Piece();
    }
    
    // Set en passant square for pawn double moves
    if (piece.type == PieceType::Pawn && abs(move.endRow - move.startRow) == 2) {
        enPassantRow = (move.startRow + move.endRow) / 2;
        enPassantCol = move.startCol;
    }
    
    // Update castling rights
    if (piece.type == PieceType::King) {
        if (piece.color == PieceColor::White) {
            whiteKingMoved = true;
        } else {
            blackKingMoved = true;
        }
    }
    
    if (piece.type == PieceType::Rook) {
        if (piece.color == PieceColor::White) {
            if (move.startCol == 0) whiteRookQueensideMoved = true;
            if (move.startCol == 7) whiteRookKingsideMoved = true;
        } else {
            if (move.startCol == 0) blackRookQueensideMoved = true;
            if (move.startCol == 7) blackRookKingsideMoved = true;
        }
    }
    
    whiteToMove = !whiteToMove;
    return true;
}

void Board::undoMove(const Move& move) {
    whiteToMove = !whiteToMove;
    
    if (move.isCastle) {
        squares[move.startRow][move.startCol] = move.movedPiece;
        squares[move.endRow][move.endCol] = Piece();
        
        if (move.endCol == 6) {
            Piece rook = squares[move.endRow][5];
            squares[move.endRow][7] = rook;
            squares[move.endRow][5] = Piece();
        } else {
            Piece rook = squares[move.endRow][3];
            squares[move.endRow][0] = rook;
            squares[move.endRow][3] = Piece();
        }
    } else if (move.isEnPassant) {
        squares[move.startRow][move.startCol] = move.movedPiece;
        squares[move.endRow][move.endCol] = Piece();
        squares[move.startRow][move.endCol] = move.capturedPiece;
    } else {
        squares[move.startRow][move.startCol] = move.movedPiece;
        squares[move.endRow][move.endCol] = move.capturedPiece;
    }
}

bool Board::isSquareAttacked(int row, int col, PieceColor attacker) {
    // Check pawn attacks
    int pawnDir = (attacker == PieceColor::White) ? 1 : -1;
    for (int dc = -1; dc <= 1; dc += 2) {
        int r = row + pawnDir;
        int c = col + dc;
        if (isValidSquare(r, c)) {
            Piece p = squares[r][c];
            if (p.type == PieceType::Pawn && p.color == attacker) {
                return true;
            }
        }
    }
    
    // Check knight attacks
    int knightOffsets[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    for (int i = 0; i < 8; i++) {
        int r = row + knightOffsets[i][0];
        int c = col + knightOffsets[i][1];
        if (isValidSquare(r, c)) {
            Piece p = squares[r][c];
            if (p.type == PieceType::Knight && p.color == attacker) {
                return true;
            }
        }
    }
    
    // Check king attacks
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int r = row + dr;
            int c = col + dc;
            if (isValidSquare(r, c)) {
                Piece p = squares[r][c];
                if (p.type == PieceType::King && p.color == attacker) {
                    return true;
                }
            }
        }
    }
    
    // Check diagonal attacks (bishop, queen)
    int diagDirs[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (int d = 0; d < 4; d++) {
        int dr = diagDirs[d][0];
        int dc = diagDirs[d][1];
        for (int dist = 1; dist < 8; dist++) {
            int r = row + dr * dist;
            int c = col + dc * dist;
            if (!isValidSquare(r, c)) break;
            
            Piece p = squares[r][c];
            if (!p.isEmpty()) {
                if (p.color == attacker && 
                    (p.type == PieceType::Bishop || p.type == PieceType::Queen)) {
                    return true;
                }
                break;
            }
        }
    }
    
    // Check straight attacks (rook, queen)
    int straightDirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (int d = 0; d < 4; d++) {
        int dr = straightDirs[d][0];
        int dc = straightDirs[d][1];
        for (int dist = 1; dist < 8; dist++) {
            int r = row + dr * dist;
            int c = col + dc * dist;
            if (!isValidSquare(r, c)) break;
            
            Piece p = squares[r][c];
            if (!p.isEmpty()) {
                if (p.color == attacker && 
                    (p.type == PieceType::Rook || p.type == PieceType::Queen)) {
                    return true;
                }
                break;
            }
        }
    }
    
    return false;
}

bool Board::inCheck(PieceColor color) {
    int kingRow, kingCol;
    findKing(color, kingRow, kingCol);
    
    PieceColor opponent = (color == PieceColor::White) ? PieceColor::Black : PieceColor::White;
    return isSquareAttacked(kingRow, kingCol, opponent);
}

void Board::findKing(PieceColor color, int& kingRow, int& kingCol) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece p = squares[r][c];
            if (p.type == PieceType::King && p.color == color) {
                kingRow = r;
                kingCol = c;
                return;
            }
        }
    }
}

bool Board::hasLegalMoves() {
    return !generateLegalMoves().empty();
}