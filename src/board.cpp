#include "board.hpp"
#include <iostream>
#include <string>
#include "move.hpp"
#include "movegen.hpp"

Board::Board() {

}

void Board::initializeFromFEN() {
    // Clear all the bitboards
    whitePawns = 0;
    whiteKnights = 0;
    whiteBishops = 0;
    whiteRooks = 0;
    whiteQueens = 0;
    whiteKing = 0;
    blackPawns = 0;
    blackKnights = 0;
    blackBishops = 0;
    blackRooks = 0;
    blackQueens = 0;
    blackKing = 0;

    // We can iterate through the rows of the FEN string and populate the bitboards accordingly
    int row = 0; // Start from the 8th row (top row of the board)
    int col = 0;

    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

    for (const char& c : fen) {
        if (c == '/') {
            // '/' indicates the end of a row, so move to the next row
            row++;
            col = 0; // Reset the column for the new row
        }
        else if (c >= '1' && c <= '8') {
            // If c is a digit between '1' and '8', it represents an empty square
            // We skip the corresponding number of squares
            col += (c - '0'); // Move the number of squares ahead
        }
        else {
            // If c is a letter, it represents a piece on the board
            // We need to find its position and set the corresponding bit in the appropriate bitboard

            // Determine the piece type and color based on the character c
            PieceType pieceType;
            PieceColor pieceColor;

            char lowerC = tolower(c);

            if (islower(c)) {
                // It's a black piece
                pieceColor = PieceColor::BLACK;
            }
            else {
                // It's a white piece
                pieceColor = PieceColor::WHITE;
            }

            switch (lowerC) {
            case 'r': pieceType = PieceType::ROOK; break;
            case 'n': pieceType = PieceType::KNIGHT; break;
            case 'b': pieceType = PieceType::BISHOP; break;
            case 'q': pieceType = PieceType::QUEEN; break;
            case 'k': pieceType = PieceType::KING; break;
            case 'p': pieceType = PieceType::PAWN; break;
            default: continue; // Ignore any other characters (like spaces)
            }

            // Calculate the bit position of the piece based on the row and column
            int position = row * 8 + col;

            // Set the bit in the appropriate bitboard based on the piece color and type
            if (pieceColor == PieceColor::WHITE) {
                switch (pieceType) {
                case PieceType::PAWN: whitePawns |= (uint64_t)1 << position; break;
                case PieceType::ROOK: whiteRooks |= (uint64_t)1 << position; break;
                case PieceType::KNIGHT: whiteKnights |= (uint64_t)1 << position; break;
                case PieceType::BISHOP: whiteBishops |= (uint64_t)1 << position; break;
                case PieceType::QUEEN: whiteQueens |= (uint64_t)1 << position; break;
                case PieceType::KING: whiteKing |= (uint64_t)1 << position; break;
                default: continue; // Ignore any other pieces (shouldn't happen)
                }
            }
            else if (pieceColor == PieceColor::BLACK) {
                switch (pieceType) {
                case PieceType::PAWN: blackPawns |= (uint64_t)1 << position; break;
                case PieceType::ROOK: blackRooks |= (uint64_t)1 << position; break;
                case PieceType::KNIGHT: blackKnights |= (uint64_t)1 << position; break;
                case PieceType::BISHOP: blackBishops |= (uint64_t)1 << position; break;
                case PieceType::QUEEN: blackQueens |= (uint64_t)1 << position; break;
                case PieceType::KING: blackKing |= (uint64_t)1 << position; break;
                default: continue; // Ignore any other pieces (shouldn't happen)
                }
            }

            // Move to the next column
            col++;
        }
    }
}


bool Board::isValidMove(int srcRow, int srcCol, int destRow, int destCol) const {
    if (!isValidPosition(srcRow, srcCol) || !isValidPosition(destRow, destCol)) {
        return false;
    }

    //Get the piece type and color of the source square
    PieceType pieceType = getPieceType(srcRow, srcCol);
    PieceColor pieceColor = getPieceColor(srcRow, srcCol);

    //Generate moves for the selected piece (*this is passing the actual board)
    std::vector<Move> movesForPiece;

    //(*this passes the actual board as an input)
    switch (pieceType) {
    case PieceType::PAWN:
        movesForPiece = generatePawnMoves(const_cast<Board&>(*this), srcRow, srcCol);
        break;

    case PieceType::ROOK:
        movesForPiece = generateRookMoves(*this, srcRow, srcCol);
        break;

    case PieceType::KNIGHT:
        movesForPiece = generateKnightMoves(*this, srcRow, srcCol);
        break;

    case PieceType::BISHOP:
        movesForPiece = generateBishopMoves(*this, srcRow, srcCol);
        break;

    case PieceType::QUEEN:
        movesForPiece = generateQueenMoves(*this, srcRow, srcCol);
        break;

    case PieceType::KING:
        movesForPiece = generateKingMoves(*this, srcRow, srcCol);
        break;

    default:
        std::cout << "Oopsies.." << std::endl;
        // Invalid piece type
        return false;
    }

    //Check if the desired move is in the list of valid moves
    for (const Move& move : movesForPiece) {
        if (move.srcRow == srcRow && move.srcCol == srcCol && move.destRow == destRow && move.destCol == destCol) {
            return true; //Move is valid
        }
    }

    return false; //Move is invalid
}

bool Board::makeMove(int srcRow, int srcCol, int destRow, int destCol) {
    if (!isValidMove(srcRow, srcCol, destRow, destCol)) {
        return false; //invalid move
    }

    //Get the piece at the source position
    PieceType pieceType = getPieceType(srcRow, srcCol);
    PieceColor pieceColor = getPieceColor(srcRow, srcCol);

    //Calculate the bit positions of the source and destination squares
    int srcPosition = srcRow * 8 + srcCol;
    int destPosition = destRow * 8 + destCol;

    //Update the appropriate bitboard to reflect the move
    uint64_t srcBitboard = (uint64_t)1 << srcPosition;
    uint64_t destBitboard = (uint64_t)1 << destPosition;

    //Clear the piece from the source square
    switch (pieceColor) {
    case PieceColor::WHITE:
        switch (pieceType) {
        case PieceType::PAWN: whitePawns &= ~srcBitboard; break;
        case PieceType::ROOK: whiteRooks &= ~srcBitboard; break;
        case PieceType::KNIGHT: whiteKnights &= ~srcBitboard; break;
        case PieceType::BISHOP: whiteBishops &= ~srcBitboard; break;
        case PieceType::QUEEN: whiteQueens &= ~srcBitboard; break;
        case PieceType::KING: whiteKing &= ~srcBitboard; break;
        default: break;
        }
        break;
    case PieceColor::BLACK:
        switch (pieceType) {
        case PieceType::PAWN: blackPawns &= ~srcBitboard; break;
        case PieceType::ROOK: blackRooks &= ~srcBitboard; break;
        case PieceType::KNIGHT: blackKnights &= ~srcBitboard; break;
        case PieceType::BISHOP: blackBishops &= ~srcBitboard; break;
        case PieceType::QUEEN: blackQueens &= ~srcBitboard; break;
        case PieceType::KING: blackKing &= ~srcBitboard; break;
        default: break;
        }
        break;
    default: break;
    }

    // If the move is a capture, remove the captured piece from the board
    if (getPieceType(destRow, destCol) != PieceType::EMPTY) {
        PieceColor capturedPieceColor = getPieceColor(destRow, destCol);

        // Calculate the bit position of the captured piece
        int capturedPiecePosition = destRow * 8 + destCol;
        uint64_t capturedPieceBitboard = (uint64_t)1 << capturedPiecePosition;

        // Clear the captured piece from the appropriate bitboard
        switch (capturedPieceColor) {
        case PieceColor::WHITE:
            whitePawns &= ~capturedPieceBitboard;
            whiteRooks &= ~capturedPieceBitboard;
            whiteKnights &= ~capturedPieceBitboard;
            whiteBishops &= ~capturedPieceBitboard;
            whiteQueens &= ~capturedPieceBitboard;
            whiteKing &= ~capturedPieceBitboard;
            break;
        case PieceColor::BLACK:
            blackPawns &= ~capturedPieceBitboard;
            blackRooks &= ~capturedPieceBitboard;
            blackKnights &= ~capturedPieceBitboard;
            blackBishops &= ~capturedPieceBitboard;
            blackQueens &= ~capturedPieceBitboard;
            blackKing &= ~capturedPieceBitboard;
            break;
        default:
            break;
        }
    }

    // Set the piece at the destination square
    switch (pieceColor) {
    case PieceColor::WHITE:
        switch (pieceType) {
        case PieceType::PAWN: whitePawns |= destBitboard; break;
        case PieceType::ROOK: whiteRooks |= destBitboard; break;
        case PieceType::KNIGHT: whiteKnights |= destBitboard; break;
        case PieceType::BISHOP: whiteBishops |= destBitboard; break;
        case PieceType::QUEEN: whiteQueens |= destBitboard; break;
        case PieceType::KING: whiteKing |= destBitboard; break;
        default: break;
        }
        break;
    case PieceColor::BLACK:
        switch (pieceType) {
        case PieceType::PAWN: blackPawns |= destBitboard; break;
        case PieceType::ROOK: blackRooks |= destBitboard; break;
        case PieceType::KNIGHT: blackKnights |= destBitboard; break;
        case PieceType::BISHOP: blackBishops |= destBitboard; break;
        case PieceType::QUEEN: blackQueens |= destBitboard; break;
        case PieceType::KING: blackKing |= destBitboard; break;
        default: break;
        }
        break;
    default: break;
    }

    return true;

}

void Board::printBitboard() const {

}

void Board::printBoard() const {
    // Create a character array to represent the chessboard
    char chessboard[8][8];

    // Initialize the chessboard array with empty squares
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            chessboard[row][col] = ' ';
        }
    }

    // Fill the chessboard array with pieces
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            PieceType pieceType = getPieceType(row, col);
            PieceColor pieceColor = getPieceColor(row, col);

            if (pieceType != PieceType::EMPTY) {
                char pieceChar;

                switch (pieceType) {
                case PieceType::PAWN:
                    pieceChar = 'p';
                    break;
                case PieceType::ROOK:
                    pieceChar = 'r';
                    break;
                case PieceType::KNIGHT:
                    pieceChar = 'n';
                    break;
                case PieceType::BISHOP:
                    pieceChar = 'b';
                    break;
                case PieceType::QUEEN:
                    pieceChar = 'q';
                    break;
                case PieceType::KING:
                    pieceChar = 'k';
                    break;
                default:
                    pieceChar = ' ';
                    break;
                }

                if (pieceColor == PieceColor::WHITE) {
                    pieceChar = toupper(pieceChar);
                }

                chessboard[row][col] = pieceChar;
            }
        }
    }

    // Print the chessboard
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            std::cout << chessboard[row][col] << " ";
        }
        std::cout << std::endl;
    }
}


PieceColor Board::getPieceColor(int row, int col) const {
    uint64_t squareBit = (uint64_t)1 << (row * 8 + col); // 8 is the BOARD_SIZE

    if (whitePawns & squareBit || whiteKnights & squareBit || whiteBishops & squareBit ||
        whiteRooks & squareBit || whiteQueens & squareBit || whiteKing & squareBit) {
        return PieceColor::WHITE;
    }
    else if (blackPawns & squareBit || blackKnights & squareBit || blackBishops & squareBit ||
        blackRooks & squareBit || blackQueens & squareBit || blackKing & squareBit) {
        return PieceColor::BLACK;
    }

    return PieceColor::EMPTY; // No piece on the square
}

PieceType Board::getPieceType(int row, int col) const {
    uint64_t squareBit = (uint64_t)1 << (row * 8 + col);

    if (whitePawns & squareBit) {
        return PieceType::PAWN;
    }
    else if (whiteKnights & squareBit) {
        return PieceType::KNIGHT;
    }
    else if (whiteBishops & squareBit) {
        return PieceType::BISHOP;
    }
    else if (whiteRooks & squareBit) {
        return PieceType::ROOK;
    }
    else if (whiteQueens & squareBit) {
        return PieceType::QUEEN;
    }
    else if (whiteKing & squareBit) {
        return PieceType::KING;
    }
    else if (blackPawns & squareBit) {
        return PieceType::PAWN;
    }
    else if (blackKnights & squareBit) {
        return PieceType::KNIGHT;
    }
    else if (blackBishops & squareBit) {
        return PieceType::BISHOP;
    }
    else if (blackRooks & squareBit) {
        return PieceType::ROOK;
    }
    else if (blackQueens & squareBit) {
        return PieceType::QUEEN;
    }
    else if (blackKing & squareBit) {
        return PieceType::KING;
    }

    return PieceType::EMPTY; // No piece on the square
}

bool Board::isEmpty(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return false;
    }
    return (getPieceType(row, col) == PieceType::EMPTY && getPieceColor(row, col) == PieceColor::EMPTY);
}

bool Board::isValidPosition(int row, int col) const {
    return row >= 0 && row < BOARD_SIZE&& col >= 0 && col < BOARD_SIZE;
}

void Board::removePiece(int row, int col, PieceColor color) {
    uint64_t squareBit = (uint64_t)1 << (row * 8 + col);

    switch (color) {
    case PieceColor::WHITE:
        whitePawns &= ~squareBit;
        whiteRooks &= ~squareBit;
        whiteKnights &= ~squareBit;
        whiteBishops &= ~squareBit;
        whiteQueens &= ~squareBit;
        whiteKing &= ~squareBit;
        break;
    case PieceColor::BLACK:
        blackPawns &= ~squareBit;
        blackRooks &= ~squareBit;
        blackKnights &= ~squareBit;
        blackBishops &= ~squareBit;
        blackQueens &= ~squareBit;
        blackKing &= ~squareBit;
        break;
    default:
        break;
    }
}


bool Board::isGameOver() const {
    // Check for checkmate or stalemate
    bool isCurrentPlayerInCheck = isInCheck();

    // Generate all possible moves for the current player
    std::vector<Move> allMoves = generateAllMoves(*this, currentPlayer);

    bool hasValidMoves = false;

    // Check if the current player has any valid moves
    for (const Move& move : allMoves) {
        Board tempBoard = *this;
        tempBoard.makeMove(move.srcRow, move.srcCol, move.destRow, move.destCol);
        if (!tempBoard.isInCheck()) {
            hasValidMoves = true;
            break;
        }
    }

    if (isCurrentPlayerInCheck && !hasValidMoves) {
        if (currentPlayer == PieceColor::WHITE) {
            std::cout << "Black wins by checkmate!" << std::endl;
        }
        else {
            std::cout << "White wins by checkmate!" << std::endl;
        }
        return true;
    }
    else if (!isCurrentPlayerInCheck && !hasValidMoves) {
        std::cout << "It's a stalemate!" << std::endl;
        return true;
    }

    return false;
}



bool Board::isInCheck() const {
    // Get the position of the current player's king
    int kingRow = -1;
    int kingCol = -1;
    PieceColor currentPlayer = getCurrentPlayer();

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (getPieceType(row, col) == PieceType::KING && getPieceColor(row, col) == currentPlayer) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }
    }

    if (kingRow == -1 || kingCol == -1) {
        // King not found (this should not happen in a valid chess position)
        return false;
    }

    // Check if any opponent's piece can attack the current player's king
    PieceColor opponentColor = (currentPlayer == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (getPieceColor(row, col) == opponentColor) {
                if (isValidMove(row, col, kingRow, kingCol)) {
                    return true;
                }
            }
        }
    }

    return false;
}


PieceColor Board::getOppositeColor(PieceColor color) const {
    return (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
}

void Board::findKing(PieceType kingType, PieceColor kingColor, int& kingRow, int& kingCol) const {
    uint64_t kingBitboard = kingColor == PieceColor::WHITE ? whiteKing : blackKing;

    // Iterate over the set bits in the kingBitboard to find the position of the king
    for (int position = 0; position < BOARD_SIZE * BOARD_SIZE; ++position) {
        uint64_t squareBit = (uint64_t)1 << position;
        if (kingBitboard & squareBit) {
            kingRow = position / BOARD_SIZE;
            kingCol = position % BOARD_SIZE;
            return;
        }
    }
}

bool Board::isCheckmate() const {
    bool isCurrentPlayerInCheck = isInCheck();
    if (!isCurrentPlayerInCheck) {
        return false;
    }

    std::vector<Move> allMoves = generateAllMoves(*this, currentPlayer);

    //Check if any of the valid moves can get the player out of check
    for (const Move& move : allMoves) {
        Board tempBoard = *this;
        tempBoard.makeMove(move.srcRow, move.srcCol, move.destRow, move.destCol);
        if (!tempBoard.isInCheck()) {
            return false; //Player can escape check, so it's not checkmate
        }
    }
    return true;
}

PieceColor Board::getCurrentPlayer() const {
    return currentPlayer;
}

void Board::setCurrentPlayer(PieceColor color) {
    currentPlayer = color;
}
