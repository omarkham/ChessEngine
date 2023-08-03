#include "search.hpp"
#include "movegen.hpp"
#include "eval.hpp"
#include "ai.hpp"
#include "board.hpp"
#include <limits>
#include "board.hpp"

Move Search::alphaBetaSearch(Board& board, int depth) {
    //Initial values for alpha and beta, representing the best possible scores for maximizing and minimizing player, respectively
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    PieceColor currentPlayerColor = board.getAIPlayer(); // AI is always the maximizing player

    std::vector<Move> allMoves = generateAllMoves(board, currentPlayerColor);

    Move bestMove;

    //Perform alpha-beta search for each possible move
    for (const Move& move : allMoves) {
        Board tempBoard = board;
        tempBoard.makeMove(move.srcRow, move.srcCol, move.destRow, move.destCol);

        //Evaluate the position after making the move
        int score = alphaBeta(tempBoard, depth - 1, alpha, beta, getOppositeColor(currentPlayerColor));

        if (score > alpha) {
            alpha = score;
            bestMove = move;
        }
    }
    return bestMove;
}


int Search::alphaBeta(Board& board, int depth, int alpha, int beta, PieceColor maximizingPlayer) {
    PieceColor color = board.getAIPlayer();
    if (depth == 0 || board.isGameOver(color)) {
        return Evaluation::evaluate(board, color); // Pass the AI player color for evaluation
    }

    std::vector<Move> allMoves = generateAllMoves(board, maximizingPlayer);

    if (allMoves.empty()) {
        // No moves available, either stalemate or checkmate
        if (board.isInCheck(color)) { // Pass the AI player color for checking
            // Checkmate, return a very negative score
            return std::numeric_limits<int>::min();
        }
        else {
            // Stalemate, return a draw score
            return 0;
        }
    }

    if (maximizingPlayer == color) {
        int maxEval = std::numeric_limits<int>::min();

        for (const Move& move : allMoves) {
            Board tempBoard = board;
            tempBoard.makeMove(move.srcRow, move.srcCol, move.destRow, move.destCol);
            int eval = alphaBeta(tempBoard, depth - 1, alpha, beta, getOppositeColor(maximizingPlayer));
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    }
    else {
        int minEval = std::numeric_limits<int>::max();

        for (const Move& move : allMoves) {
            Board tempBoard = board;
            tempBoard.makeMove(move.srcRow, move.srcCol, move.destRow, move.destCol);
            int eval = alphaBeta(tempBoard, depth - 1, alpha, beta, getOppositeColor(maximizingPlayer));
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}
