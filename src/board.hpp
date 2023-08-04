#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include "piece.hpp"
#include "move.hpp"

class Board {
public:
    const int BOARD_SIZE = 8;

    Board();
    void initializeFromFEN();
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol) const;
    bool makeMove(int srcRow, int srcCol, int destRow, int destCol);
    void printBitboard() const;
    void printBoard() const;
    bool isValidPosition(int row, int col) const;
    bool isEmpty(int row, int col) const;
    void removePiece(int row, int col, PieceColor color);
    bool isGameOver(PieceColor color) const;
    bool isInCheck(PieceColor color) const;
    bool isCheckmate(PieceColor color) const;
    void findKing(PieceType kingType, PieceColor kingColor, int& kingRow, int& kingCol) const;

    PieceColor getOppositeColor(PieceColor color) const;
    PieceType getPieceType(int row, int col) const;
    PieceColor getPieceColor(int row, int col) const;

    PieceColor getAIPlayer() const { return aiPlayer; }
    PieceColor getRealPlayer() const { return realPlayer;  }
    void setAIPlayer(PieceColor color) { aiPlayer = color;  }
    void setRealPlayer(PieceColor color) { realPlayer = color;  }

private:

    uint64_t whitePawns;
    uint64_t whiteKnights;
    uint64_t whiteBishops;
    uint64_t whiteRooks;
    uint64_t whiteQueens;
    uint64_t whiteKing;
    uint64_t blackPawns;
    uint64_t blackKnights;
    uint64_t blackBishops;
    uint64_t blackRooks;
    uint64_t blackQueens;
    uint64_t blackKing;

    PieceColor aiPlayer;
    PieceColor realPlayer;

};

#endif //BOARD_HPP
