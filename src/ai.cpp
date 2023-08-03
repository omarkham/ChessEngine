#include "ai.hpp"
#include "search.hpp"
#include "eval.hpp"

Move AI::findBestMove(Board& board, int depth) {
    Search search;
    return search.alphaBetaSearch(board, depth);
}
