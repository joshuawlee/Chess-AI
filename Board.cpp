#include "Board.h"

Board::Board() : whiteToMove(true), whiteKingMoved(false), blackKingMoved(false),
                 whiteRookKingsideMoved(false), whiteRookQueensideMoved(false),
                 blackRookKingsideMoved(false), blackRookQueensideMoved(false),
                 enPassantCol(-1), enPassantRow(-1) {
    initializeBoard();
}

void Board::initializeBoard() {
    // Initialize empty squares
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            squares[i][j] = {PieceType::None, PieceColor::NoColor};
        }
    }

    // Black pieces (top of board, row 0-1)
    squares[0][0] = {PieceType::Rook, PieceColor::Black};
    squares[0][1] = {PieceType::Knight, PieceColor::Black};
    squares[0][2] = {PieceType::Bishop, PieceColor::Black};
    squares[0][3] = {PieceType::Queen, PieceColor::Black};
    squares[0][4] = {PieceType::King, PieceColor::Black};
    squares[0][5] = {PieceType::Bishop, PieceColor::Black};
    squares[0][6] = {PieceType::Knight, PieceColor::Black};
    squares[0][7] = {PieceType::Rook, PieceColor::Black};
    
    for(int i = 0; i < 8; i++) {
        squares[1][i] = {PieceType::Pawn, PieceColor::Black};
    }

    // White pieces (bottom of board, row 6-7)
    for(int i = 0; i < 8; i++) {
        squares[6][i] = {PieceType::Pawn, PieceColor::White};
    }
    
    squares[7][0] = {PieceType::Rook, PieceColor::White};
    squares[7][1] = {PieceType::Knight, PieceColor::White};
    squares[7][2] = {PieceType::Bishop, PieceColor::White};
    squares[7][3] = {PieceType::Queen, PieceColor::White};
    squares[7][4] = {PieceType::King, PieceColor::White};
    squares[7][5] = {PieceType::Bishop, PieceColor::White};
    squares[7][6] = {PieceType::Knight, PieceColor::White};
    squares[7][7] = {PieceType::Rook, PieceColor::White};
}

bool Board::isValidSquare(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

Piece Board::getPiece(int row, int col) const {
    if(isValidSquare(row, col)) {
        return squares[row][col];
    }
    return {PieceType::None, PieceColor::NoColor};
}

void Board::setPiece(int row, int col, Piece piece) {
    if(isValidSquare(row, col)) {
        squares[row][col] = piece;
    }
}

std::vector<Move> Board::generatePseudoLegalMoves() {
    std::vector<Move> moves;
    PieceColor currentColor = whiteToMove ? PieceColor::White : PieceColor::Black;

    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            Piece piece = squares[row][col];
            if(piece.color == currentColor) {
                switch(piece.type) {
                    case PieceType::Pawn:
                        generatePawnMoves(row, col, moves);
                        break;
                    case PieceType::Knight:
                        generateKnightMoves(row, col, moves);
                        break;
                    case PieceType::Bishop:
                        generateBishopMoves(row, col, moves);
                        break;
                    case PieceType::Rook:
                        generateRookMoves(row, col, moves);
                        break;
                    case PieceType::Queen:
                        generateQueenMoves(row, col, moves);
                        break;
                    case PieceType::King:
                        generateKingMoves(row, col, moves);
                        break;
                    default:
                        break;
                }
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
    if(isValidSquare(newRow, col) && squares[newRow][col].type == PieceType::None) {
        Move move;
        move.startRow = row;
        move.startCol = col;
        move.endRow = newRow;
        move.endCol = col;
        move.movedPiece = pawn;
        move.capturedPiece = {PieceType::None, PieceColor::NoColor};
        move.isPromotion = (newRow == promotionRow);
        move.promotionType = PieceType::Queen; // Default to queen
        move.isCastle = false;
        move.isEnPassant = false;
        moves.push_back(move);

        // Double move from start
        if(row == startRow) {
            int doubleRow = row + 2 * direction;
            if(squares[doubleRow][col].type == PieceType::None) {
                Move doubleMove = move;
                doubleMove.endRow = doubleRow;
                moves.push_back(doubleMove);
            }
        }
    }

    // Captures
    for(int dcol : {-1, 1}) {
        int newCol = col + dcol;
        if(isValidSquare(newRow, newCol)) {
            Piece target = squares[newRow][newCol];
            if(target.type != PieceType::None && target.color != pawn.color) {
                Move move;
                move.startRow = row;
                move.startCol = col;
                move.endRow = newRow;
                move.endCol = newCol;
                move.movedPiece = pawn;
                move.capturedPiece = target;
                move.isPromotion = (newRow == promotionRow);
                move.promotionType = PieceType::Queen;
                move.isCastle = false;
                move.isEnPassant = false;
                moves.push_back(move);
            }
            // En passant
            else if(newCol == enPassantCol && newRow == enPassantRow) {
                Move move;
                move.startRow = row;
                move.startCol = col;
                move.endRow = newRow;
                move.endCol = newCol;
                move.movedPiece = pawn;
                move.capturedPiece = {PieceType::Pawn, (pawn.color == PieceColor::White) ? PieceColor::Black : PieceColor::White};
                move.isPromotion = false;
                move.isCastle = false;
                move.isEnPassant = true;
                moves.push_back(move);
            }
        }
    }
}

void Board::generateKnightMoves(int row, int col, std::vector<Move>& moves) {
    Piece knight = squares[row][col];
    int knightMoves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};

    for(auto& km : knightMoves) {
        int newRow = row + km[0];
        int newCol = col + km[1];
        if(isValidSquare(newRow, newCol)) {
            Piece target = squares[newRow][newCol];
            if(target.type == PieceType::None || target.color != knight.color) {
                Move move;
                move.startRow = row;
                move.startCol = col;
                move.endRow = newRow;
                move.endCol = newCol;
                move.movedPiece = knight;
                move.capturedPiece = target;
                move.isPromotion = false;
                move.isCastle = false;
                move.isEnPassant = false;
                moves.push_back(move);
            }
        }
    }
}

void Board::generateSlidingMoves(int row, int col, std::vector<Move>& moves,
                                const std::vector<std::pair<int,int>>& directions) {
    Piece piece = squares[row][col];

    for(auto& dir : directions) {
        int newRow = row + dir.first;
        int newCol = col + dir.second;

        while(isValidSquare(newRow, newCol)) {
            Piece target = squares[newRow][newCol];
            if(target.type == PieceType::None) {
                Move move;
                move.startRow = row;
                move.startCol = col;
                move.endRow = newRow;
                move.endCol = newCol;
                move.movedPiece = piece;
                move.capturedPiece = target;
                move.isPromotion = false;
                move.isCastle = false;
                move.isEnPassant = false;
                moves.push_back(move);
            } else {
                if(target.color != piece.color) {
                    Move move;
                    move.startRow = row;
                    move.startCol = col;
                    move.endRow = newRow;
                    move.endCol = newCol;
                    move.movedPiece = piece;
                    move.capturedPiece = target;
                    move.isPromotion = false;
                    move.isCastle = false;
                    move.isEnPassant = false;
                    moves.push_back(move);
                }
                break;
            }
            newRow += dir.first;
            newCol += dir.second;
        }
    }
}

void Board::generateBishopMoves(int row, int col, std::vector<Move>& moves) {
    std::vector<std::pair<int,int>> diagonals = {{-1,-1},{-1,1},{1,-1},{1,1}};
    generateSlidingMoves(row, col, moves, diagonals);
}

void Board::generateRookMoves(int row, int col, std::vector<Move>& moves) {
    std::vector<std::pair<int,int>> orthogonals = {{-1,0},{1,0},{0,-1},{0,1}};
    generateSlidingMoves(row, col, moves, orthogonals);
}

void Board::generateQueenMoves(int row, int col, std::vector<Move>& moves) {
    std::vector<std::pair<int,int>> allDirections = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    generateSlidingMoves(row, col, moves, allDirections);
}

void Board::generateKingMoves(int row, int col, std::vector<Move>& moves) {
    Piece king = squares[row][col];
    int kingMoves[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};

    for(auto& km : kingMoves) {
        int newRow = row + km[0];
        int newCol = col + km[1];
        if(isValidSquare(newRow, newCol)) {
            Piece target = squares[newRow][newCol];
            if(target.type == PieceType::None || target.color != king.color) {
                Move move;
                move.startRow = row;
                move.startCol = col;
                move.endRow = newRow;
                move.endCol = newCol;
                move.movedPiece = king;
                move.capturedPiece = target;
                move.isPromotion = false;
                move.isCastle = false;
                move.isEnPassant = false;
                moves.push_back(move);
            }
        }
    }

    // Castling
    bool kingMoved = (king.color == PieceColor::White) ? whiteKingMoved : blackKingMoved;
    if(!kingMoved && !isInCheck(king.color)) {
        int backRow = (king.color == PieceColor::White) ? 7 : 0;
        
        // Kingside castling
        bool kingsideRookMoved = (king.color == PieceColor::White) ? whiteRookKingsideMoved : blackRookKingsideMoved;
        if(!kingsideRookMoved && 
           squares[backRow][5].type == PieceType::None &&
           squares[backRow][6].type == PieceType::None &&
           !isSquareAttacked(backRow, 5, (king.color == PieceColor::White) ? PieceColor::Black : PieceColor::White) &&
           !isSquareAttacked(backRow, 6, (king.color == PieceColor::White) ? PieceColor::Black : PieceColor::White)) {
            Move castle;
            castle.startRow = row;
            castle.startCol = col;
            castle.endRow = backRow;
            castle.endCol = 6;
            castle.movedPiece = king;
            castle.capturedPiece = {PieceType::None, PieceColor::NoColor};
            castle.isPromotion = false;
            castle.isCastle = true;
            castle.isEnPassant = false;
            moves.push_back(castle);
        }

        // Queenside castling
        bool queensideRookMoved = (king.color == PieceColor::White) ? whiteRookQueensideMoved : blackRookQueensideMoved;
        if(!queensideRookMoved &&
           squares[backRow][1].type == PieceType::None &&
           squares[backRow][2].type == PieceType::None &&
           squares[backRow][3].type == PieceType::None &&
           !isSquareAttacked(backRow, 2, (king.color == PieceColor::White) ? PieceColor::Black : PieceColor::White) &&
           !isSquareAttacked(backRow, 3, (king.color == PieceColor::White) ? PieceColor::Black : PieceColor::White)) {
            Move castle;
            castle.startRow = row;
            castle.startCol = col;
            castle.endRow = backRow;
            castle.endCol = 2;
            castle.movedPiece = king;
            castle.capturedPiece = {PieceType::None, PieceColor::NoColor};
            castle.isPromotion = false;
            castle.isCastle = true;
            castle.isEnPassant = false;
            moves.push_back(castle);
        }
    }
}

bool Board::isSquareAttacked(int row, int col, PieceColor attacker) {
    // Check for pawn attacks
    int pawnDir = (attacker == PieceColor::White) ? 1 : -1;
    for(int dcol : {-1, 1}) {
        int pawnRow = row + pawnDir;
        int pawnCol = col + dcol;
        if(isValidSquare(pawnRow, pawnCol)) {
            Piece p = squares[pawnRow][pawnCol];
            if(p.type == PieceType::Pawn && p.color == attacker) {
                return true;
            }
        }
    }

    // Check for knight attacks
    int knightMoves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for(auto& km : knightMoves) {
        int kr = row + km[0];
        int kc = col + km[1];
        if(isValidSquare(kr, kc)) {
            Piece p = squares[kr][kc];
            if(p.type == PieceType::Knight && p.color == attacker) {
                return true;
            }
        }
    }

    // Check for bishop/queen diagonal attacks
    std::vector<std::pair<int,int>> diagonals = {{-1,-1},{-1,1},{1,-1},{1,1}};
    for(auto& dir : diagonals) {
        int r = row + dir.first;
        int c = col + dir.second;
        while(isValidSquare(r, c)) {
            Piece p = squares[r][c];
            if(p.type != PieceType::None) {
                if(p.color == attacker && (p.type == PieceType::Bishop || p.type == PieceType::Queen)) {
                    return true;
                }
                break;
            }
            r += dir.first;
            c += dir.second;
        }
    }

    // Check for rook/queen orthogonal attacks
    std::vector<std::pair<int,int>> orthogonals = {{-1,0},{1,0},{0,-1},{0,1}};
    for(auto& dir : orthogonals) {
        int r = row + dir.first;
        int c = col + dir.second;
        while(isValidSquare(r, c)) {
            Piece p = squares[r][c];
            if(p.type != PieceType::None) {
                if(p.color == attacker && (p.type == PieceType::Rook || p.type == PieceType::Queen)) {
                    return true;
                }
                break;
            }
            r += dir.first;
            c += dir.second;
        }
    }

    // Check for king attacks
    int kingMoves[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    for(auto& km : kingMoves) {
        int kr = row + km[0];
        int kc = col + km[1];
        if(isValidSquare(kr, kc)) {
            Piece p = squares[kr][kc];
            if(p.type == PieceType::King && p.color == attacker) {
                return true;
            }
        }
    }

    return false;
}

bool Board::isInCheck(PieceColor color) {
    // Find king position
    int kingRow = -1, kingCol = -1;
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            Piece p = squares[row][col];
            if(p.type == PieceType::King && p.color == color) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }
        if(kingRow != -1) break;
    }

    if(kingRow == -1) return false; // King not found

    PieceColor opponent = (color == PieceColor::White) ? PieceColor::Black : PieceColor::White;
    return isSquareAttacked(kingRow, kingCol, opponent);
}

bool Board::makeMove(const Move& move) {
    // Handle en passant capture
    if(move.isEnPassant) {
        int capturedPawnRow = (move.movedPiece.color == PieceColor::White) ? move.endRow + 1 : move.endRow - 1;
        squares[capturedPawnRow][move.endCol] = {PieceType::None, PieceColor::NoColor};
    }

    // Handle castling
    if(move.isCastle) {
        int backRow = move.startRow;
        if(move.endCol == 6) { // Kingside
            squares[backRow][5] = squares[backRow][7];
            squares[backRow][7] = {PieceType::None, PieceColor::NoColor};
        } else if(move.endCol == 2) { // Queenside
            squares[backRow][3] = squares[backRow][0];
            squares[backRow][0] = {PieceType::None, PieceColor::NoColor};
        }
    }

    // Move piece
    squares[move.endRow][move.endCol] = move.movedPiece;
    squares[move.startRow][move.startCol] = {PieceType::None, PieceColor::NoColor};

    // Handle promotion
    if(move.isPromotion) {
        squares[move.endRow][move.endCol].type = move.promotionType;
    }

    // Update castling rights
    if(move.movedPiece.type == PieceType::King) {
        if(move.movedPiece.color == PieceColor::White) whiteKingMoved = true;
        else blackKingMoved = true;
    }
    if(move.movedPiece.type == PieceType::Rook) {
        if(move.movedPiece.color == PieceColor::White) {
            if(move.startCol == 0) whiteRookQueensideMoved = true;
            if(move.startCol == 7) whiteRookKingsideMoved = true;
        } else {
            if(move.startCol == 0) blackRookQueensideMoved = true;
            if(move.startCol == 7) blackRookKingsideMoved = true;
        }
    }

    // Update en passant
    enPassantCol = -1;
    enPassantRow = -1;
    if(move.movedPiece.type == PieceType::Pawn && abs(move.endRow - move.startRow) == 2) {
        enPassantCol = move.endCol;
        enPassantRow = (move.startRow + move.endRow) / 2;
    }

    whiteToMove = !whiteToMove;
    return true;
}

void Board::undoMove(const Move& move) {
    // Restore moved piece
    squares[move.startRow][move.startCol] = move.movedPiece;
    
    // Restore captured piece or clear destination
    if(move.isEnPassant) {
        int capturedPawnRow = (move.movedPiece.color == PieceColor::White) ? move.endRow + 1 : move.endRow - 1;
        squares[capturedPawnRow][move.endCol] = move.capturedPiece;
        squares[move.endRow][move.endCol] = {PieceType::None, PieceColor::NoColor};
    } else {
        squares[move.endRow][move.endCol] = move.capturedPiece;
    }

    // Undo castling
    if(move.isCastle) {
        int backRow = move.startRow;
        if(move.endCol == 6) { // Kingside
            squares[backRow][7] = squares[backRow][5];
            squares[backRow][5] = {PieceType::None, PieceColor::NoColor};
        } else if(move.endCol == 2) { // Queenside
            squares[backRow][0] = squares[backRow][3];
            squares[backRow][3] = {PieceType::None, PieceColor::NoColor};
        }
    }

    whiteToMove = !whiteToMove;
}

std::vector<Move> Board::generateLegalMoves() {
    std::vector<Move> pseudoLegal = generatePseudoLegalMoves();
    std::vector<Move> legal;

    PieceColor currentColor = whiteToMove ? PieceColor::White : PieceColor::Black;

    for(const Move& move : pseudoLegal) {
        // Save state
        bool savedWhiteKingMoved = whiteKingMoved;
        bool savedBlackKingMoved = blackKingMoved;
        bool savedWhiteRookKMoved = whiteRookKingsideMoved;
        bool savedWhiteRookQMoved = whiteRookQueensideMoved;
        bool savedBlackRookKMoved = blackRookKingsideMoved;
        bool savedBlackRookQMoved = blackRookQueensideMoved;
        int savedEnPassantCol = enPassantCol;
        int savedEnPassantRow = enPassantRow;

        makeMove(move);
        
        if(!isInCheck(currentColor)) {
            legal.push_back(move);
        }

        undoMove(move);

        // Restore state
        whiteKingMoved = savedWhiteKingMoved;
        blackKingMoved = savedBlackKingMoved;
        whiteRookKingsideMoved = savedWhiteRookKMoved;
        whiteRookQueensideMoved = savedWhiteRookQMoved;
        blackRookKingsideMoved = savedBlackRookKMoved;
        blackRookQueensideMoved = savedBlackRookQMoved;
        enPassantCol = savedEnPassantCol;
        enPassantRow = savedEnPassantRow;
    }

    return legal;
}

bool Board::isCheckmate() {
    PieceColor currentColor = whiteToMove ? PieceColor::White : PieceColor::Black;
    return isInCheck(currentColor) && generateLegalMoves().empty();
}

bool Board::isStalemate() {
    PieceColor currentColor = whiteToMove ? PieceColor::White : PieceColor::Black;
    return !isInCheck(currentColor) && generateLegalMoves().empty();
}