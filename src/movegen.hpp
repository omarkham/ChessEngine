#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include "board.hpp"


// Forward declarations for the piece-specific move generation functions
std::vector<Move> generatePawnMoves(const Board& board, int srcRow, int srcCol);
std::vector<Move> generateRookMoves(const Board& board, int srcRow, int srcCol);
std::vector<Move> generateKnightMoves(const Board& board, int srcRow, int srcCol);
std::vector<Move> generateBishopMoves(const Board& board, int srcRow, int srcCol);
std::vector<Move> generateQueenMoves(const Board& board, int srcRow, int srcCol);
std::vector<Move> generateKingMoves(const Board& board, int srcRow, int srcCol);

std::vector<Move> generateMovesForPiece(const Board& board, int row, int col);
std::vector<Move> generateMovesForColor(const Board& board, PieceColor color);

PieceColor getOppositeColor(PieceColor color);

#endif // MOVEGEN_HPP
