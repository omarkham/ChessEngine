#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "board.hpp"
#include "move.hpp"

class Search {
public:
	Move alphaBetaSearch(Board& board, int depth);
	int alphaBeta(Board& board, int depth, int alpha, int beta, PieceColor maximizingPlayer);
};

#endif // SEARCH_HPP