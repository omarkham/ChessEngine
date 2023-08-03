#include "mcts.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iostream>

MCTS::Node::Node(Node* parent, const Move& move, Board* board)
    : parent(parent), children(), visits(0), wins(0.0), move(move), board(board) {}

MCTS::Node::~Node() {
    for (Node* child : children) {
        delete child;
    }
}

MCTS::MCTS(int timeLimitMs) {
    timeLimit = timeLimitMs;
}

Move MCTS::findBestMove(const Board& board) {
    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout << "AI's turn, beginning now" << std::endl;

    Node rootNode(nullptr, Move(), new Board(board));
    rootNode.visits = 1;

    // Print out possible moves for the AI before starting MCTS
    std::vector<Move> possibleMoves = generateMovesForColor(board, PieceColor::BLACK);
    std::cout << "Possible moves for AI:" << std::endl;
    for (const Move& move : possibleMoves) {
        std::cout << "From: (" << move.srcRow << ", " << move.srcCol << ") To: (" << move.destRow << ", " << move.destCol << ")" << std::endl;
    }

    // Check if there are possible moves for the AI
    if (!possibleMoves.empty()) {
        rootNode.move = possibleMoves[0]; // Set the move for the initial rootNode
    }

    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count() < timeLimit) {
        Node* selectedNode = select(&rootNode);
        if (!selectedNode) {
            std::cout << "No valid children for rootNode. Stopping simulation." << std::endl;
            break;
        }

        Node* expandedNode = expand(selectedNode);
        if (!expandedNode) {
            std::cout << "No valid moves from selected node. Skipping simulation." << std::endl;
            continue;
        }

        double simulationResult = simulate(*(expandedNode->board), getOppositeColor(rootNode.board->getCurrentPlayer()));
        backpropagate(expandedNode, simulationResult);
    }

    Node* bestChild = nullptr;
    double bestScore = -1.0;

    // Check if rootNode has children
    if (!rootNode.children.empty()) {
        for (Node* child : rootNode.children) {
            double score = child->wins / child->visits;
            if (score > bestScore) {
                bestChild = child;
                bestScore = score;
            }
        }

        if (bestChild) {
            Move bestMove = bestChild->move;
            std::cout << "AI selected move: "
                << "Type: " << static_cast<int>(board.getPieceType(bestMove.srcRow, bestMove.srcCol)) << " "
                << "Color: " << static_cast<int>(board.getPieceColor(bestMove.srcRow, bestMove.srcCol)) << " "
                << "SrcRow: " << bestMove.srcRow << " "
                << "SrcCol: " << bestMove.srcCol << " "
                << "DestRow: " << bestMove.destRow << " "
                << "DestCol: " << bestMove.destCol << std::endl;

            std::cout << "AI move generation took: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count()
                << " ms" << std::endl;

            return bestMove;
        }
    }
    std::cout << "No legal moves found. Game ended or no children for rootNode." << std::endl;
    return Move(0, 0, 0, 0);
}

MCTS::Node* MCTS::select(Node* node) {
    while (!node->children.empty()) {
        double bestScore = -1.0;
        Node* bestChild = nullptr;

        for (Node* child : node->children) {
            double score = child->wins / child->visits + std::sqrt(2.0 * std::log(static_cast<double>(node->visits)) / child->visits);
            if (score > bestScore) {
                bestScore = score;
                bestChild = child;
            }
        }

        // If there are no children, expand the node and return one of its children
        if (!bestChild) {
            bestChild = expand(node);
            break;
        }

        node = bestChild;
    }

    return node;
}

MCTS::Node* MCTS::expand(Node* node) {
    std::cout << "Beginning expansion" << std::endl;
    // Generate legal moves for the current board state
    std::vector<Move> legalMoves = generateMovesForPiece(*(node->board), node->move.srcRow, node->move.srcCol);

    std::cout << "Expanded possible moves for AI: " << std::endl;
    for (const Move& move : legalMoves) {
        std::cout << "From: (" << move.srcRow << ", " << move.srcCol << ") To: (" << move.destRow << ", " << move.destCol << ")" << std::endl;
    }

    std::cout << "Beginning to make child nodes for each legal move" << std::endl;

    // Add a print statement to see which node is being expanded and the number of legal moves found
    std::cout << "Expanding node at: (" << node->move.srcRow << ", " << node->move.srcCol << "), Legal moves found: " << legalMoves.size() << std::endl;

    // Create child nodes for each legal move
    for (const Move& move : legalMoves) {
        Board* newBoard = new Board(*(node->board));
        newBoard->makeMove(node->move.srcRow, node->move.srcCol, move.destRow, move.destCol);
        newBoard->getCurrentPlayer();
        node->children.push_back(new Node(node, move, newBoard));

        // Add print statements to see the details of each child node
        std::cout << "Child Node - Move: "
            << "Type: " << static_cast<int>(newBoard->getPieceType(move.srcRow, move.srcCol)) << " "
            << "Color: " << static_cast<int>(newBoard->getPieceColor(move.srcRow, move.srcCol)) << " "
            << "SrcRow: " << move.srcRow << " "
            << "SrcCol: " << move.srcCol << " "
            << "DestRow: " << move.destRow << " "
            << "DestCol: " << move.destCol << std::endl;
        // Optionally, you can also print the board state after each move
        newBoard->printBoard();
    }

    // Randomly select and return one of the children nodes
    if (!node->children.empty()) {
        int randomChildIndex = std::rand() % node->children.size();
        return node->children[randomChildIndex];
    }

    return nullptr; // Return nullptr if no valid children are generated
}

double MCTS::simulate(Board& board, PieceColor currentPlayer) {
    std::cout << "Simulation start:" << std::endl;
    Board tempBoard = board;

    // Perform random playouts until the game is over or the max depth is reached
    int maxDepth = 200; // Set a reasonable maximum depth to avoid infinite loops
    int currentDepth = 0;

    while (!tempBoard.isGameOver() && currentDepth < maxDepth) {
        // Generate moves for the current board state based on the currentPlayer
        std::vector<Move> legalMoves = generateMovesForColor(tempBoard, currentPlayer);
        if (legalMoves.empty()) {
            break;
        }

        // Randomly select a move from the available legal moves
        int randomMoveIndex = std::rand() % legalMoves.size();
        const Move& randomMove = legalMoves[randomMoveIndex];
        tempBoard.makeMove(randomMove.srcRow, randomMove.srcCol, randomMove.destRow, randomMove.destCol);

        // Toggle the currentPlayer for the next turn
        currentPlayer = getOppositeColor(currentPlayer);

        // Add debugging print statement to see the board after each move during simulation
        std::cout << "Simulating move: " << randomMove.srcRow << ", " << randomMove.srcCol << " to "
            << randomMove.destRow << ", " << randomMove.destCol << std::endl;
        tempBoard.printBoard();

        currentDepth++;
    }

    // Return the result of the playout (1.0 for win, 0.5 for draw, 0.0 for loss)
    if (tempBoard.isCheckmate(getOppositeColor(currentPlayer))) {
        return currentPlayer == PieceColor::WHITE ? 0.0 : 1.0;
    }
    else if (tempBoard.isStalemate() || tempBoard.isInsufficientMaterials() || tempBoard.isThreefoldRepetition()) {
        return 0.5;
    }
    else {
        return 0.0;
    }
}

void MCTS::backpropagate(Node* node, double result) {
    while (node != nullptr) {
        node->visits++;
        node->wins += result;

        // Add debugging print statement to see the updated wins and visits for each node during backpropagation
        std::cout << "Backpropagating node: " << node->move.srcRow << ", " << node->move.srcCol << " to "
            << node->move.destRow << ", " << node->move.destCol << std::endl;
        std::cout << "Wins: " << node->wins << ", Visits: " << node->visits << std::endl;

        node = node->parent;
    }
}
