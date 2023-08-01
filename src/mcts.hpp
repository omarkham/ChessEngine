#ifndef MCTS_HPP
#define MCTS_HPP

#include "board.hpp"
#include "movegen.hpp"
#include <vector>
#include <ctime>

class MCTS {
public:
    // Constructors
    MCTS() : timeLimit(5000) {}
    MCTS(int timeLimitMs);

    // Perform MCTS and return the best move for the current player
    Move findBestMove(const Board& board);

private:
    struct Node {
        Node* parent;
        std::vector<Node*> children;
        int visits;
        double wins;
        Move move;
        Board* board; // Use pointer to Board instead of object

        Node(Node* parent, const Move& move, Board* board); // Update the constructor
        ~Node();
    };

    // Private helper functions
    Node* select(Node* node);
    Node* expand(Node* node);
    double simulate(Board& board, PieceColor color);
    void backpropagate(Node* node, double result);

    int timeLimit;
};

#endif // MCTS_HPP
