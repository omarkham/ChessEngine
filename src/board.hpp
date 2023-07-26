#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include "piece.hpp"

class Board {
public:
    const int BOARD_SIZE = 8;

	Board();
	void initializeFromFEN();
	bool isValidMove(int srcRow, int srcCol, int destRow, int destCol) const;
	bool makeMove(int srcRow, int srcCol, int destRow, int destCol);
	void printBitboard() const;
    void printBoard() const;
	PieceType getPieceType(int row, int col) const;
	PieceColor getPieceColor(int row, int col) const;
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

};

#endif //BOARD_HPP
