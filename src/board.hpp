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
    bool isCheckmate(PieceColor color) const;
    bool isCheck(PieceColor color) const;
    bool isSquareAttacked(int row, int col, PieceColor attackingColor) const;
    bool isStalemate() const;
    bool isGameOver() const;
    bool isInsufficientMaterials() const;
    bool isThreefoldRepetition() const;
    void makeAIMove();

    PieceColor getCurrentPlayer() const;
	PieceType getPieceType(int row, int col) const;
	PieceColor getPieceColor(int row, int col) const;

    //Add a copy constructor and assignment operator
    Board(const Board& board) = default;
    Board& operator=(const Board& other) = delete;

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

    mutable std::vector<uint64_t> previousBoardStates;

    static int popcount64(uint64_t n);

    PieceColor currentPlayer;
};

#endif //BOARD_HPP
