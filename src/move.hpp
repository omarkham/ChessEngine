#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>

// Enum to represent additional flags for special moves
enum class MoveType {
    QUIET,     // Quiet move (non-capturing)
    CAPTURE,   // Capture move
    // Add other move types as needed, e.g., EN_PASSANT, PROMOTION, CASTLING, etc.
};

// Define a struct to represent a chess move
struct Move {
    int srcRow;
    int srcCol;
    int destRow;
    int destCol;
    MoveType flags; // Additional flags to represent special moves

    // Constructor to initialize a move with default values
    Move() : srcRow(-1), srcCol(-1), destRow(-1), destCol(-1), flags(MoveType::QUIET) {}

    // Constructor to initialize a move with specific values
    Move(int srcRow, int srcCol, int destRow, int destCol, MoveType flags = MoveType::QUIET)
        : srcRow(srcRow), srcCol(srcCol), destRow(destRow), destCol(destCol), flags(flags) {}
};

#endif // MOVE_HPP