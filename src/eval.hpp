#ifndef EVAL_HPP
#define EVAL_HPP

#include "board.hpp"

class Evaluation {
public:
	static int evaluate(const Board& board, PieceColor color);
};

#endif // EVAL_HPP
