#ifndef AI_HPP
#define AI_HPP

#include "board.hpp"
#include "move.hpp"

class AI {
public:
	static Move findBestMove(Board& board, int depth);
};

#endif // AI_HPP