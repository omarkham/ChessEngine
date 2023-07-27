// movegen.cpp

#include "movegen.hpp"
#include "piece.hpp"
#include "move.hpp"
#include "board.hpp"


// Function to generate moves for a pawn
std::vector<Move> generatePawnMoves(const Board & board, int srcRow, int srcCol) {
    std::vector<Move> moves;

    PieceColor pieceColor = board.getPieceColor(srcRow, srcCol);

    int forwardDirection = (pieceColor == PieceColor::WHITE) ? -1 : 1;

    //Check the single square advance
    int destRow = srcRow + forwardDirection;
    int destCol = srcCol;
    if (board.isEmpty(destRow, destCol)) {
        moves.push_back({ srcRow, srcCol, destRow, destCol, MoveType::QUIET });
    }

    //Check the double square advance (only if the pawn is in it's starting position)
    if ((pieceColor == PieceColor::WHITE && srcRow == 6) || (pieceColor == PieceColor::BLACK && srcRow == 1)) {
        destRow = srcRow + 2 * forwardDirection;
        destCol = srcCol;
        if (board.isEmpty(destRow, destCol) && board.isEmpty(destRow - forwardDirection, destCol)) {
            moves.push_back({ srcRow, srcCol, destRow, destCol, MoveType::QUIET }); 

        }
    }

    // Check for diagonal capturing moves
    auto tryCaptureMove = [&](int targetRow, int targetCol) {
        if (board.isValidPosition(targetRow, targetCol)) {
            PieceType targetPieceType = board.getPieceType(targetRow, targetCol);
            PieceColor targetPieceColor = board.getPieceColor(targetRow, targetCol);
            if (targetPieceType != PieceType::EMPTY && targetPieceColor != pieceColor) {
                moves.push_back({ srcRow, srcCol, targetRow, targetCol, MoveType::CAPTURE });
            }
        }
    };

    // Capture to the left
    tryCaptureMove(srcRow + forwardDirection, srcCol - 1);

    // Capture to the right
    tryCaptureMove(srcRow + forwardDirection, srcCol + 1);

    // TODO: Implement en passant logic

    return moves;
}

// Function to generate moves for a rook
std::vector<Move> generateRookMoves(const Board& board, int srcRow, int srcCol) {
    std::vector<Move> moves;

    //Define the four cardinal directions: up, down, left, right
    int dr[] = { -1, 1, 0, 0 };
    int dc[] = { 0, 0, -1, 1 };

    PieceColor pieceColor = board.getPieceColor(srcRow, srcCol);

    //Check moves in each of the four directions
    for (int i = 0; i < 4; ++i) {
        int destRow = srcRow + dr[i];
        int destCol = srcCol + dc[i];

        //Keep extending the move in the current direction until we reach the edge of the board or another piece
        while (board.isValidPosition(destRow, destCol)) {
            PieceColor targetPieceColor = board.getPieceColor(destRow, destCol);

            //If the destination square is empty, it's a quiet move
            if (targetPieceColor == PieceColor::EMPTY) {
                moves.push_back({ srcRow, srcCol, destRow, destCol, MoveType::QUIET });
            }
            //If the destination square has an opponent's piece, it's a capture move
            else if (targetPieceColor != pieceColor) {
                moves.push_back({ srcRow, srcCol, destRow, destCol, MoveType::CAPTURE });
                break;// Can't go beyond a capture piece
            }
            else {
                //If the destination square has a piece of the same color, we cannot move beyond it
                break;
            }

            //Move further along the current direction
            destRow += dr[i];
            destCol += dc[i];
        }
    }

    return moves;
}

// Function to generate moves for a knight
std::vector<Move> generateKnightMoves(const Board& board, int srcRow, int srcCol) {
    std::vector<Move> moves;

    PieceColor pieceColor = board.getPieceColor(srcRow, srcCol);

    int dr[] = { -2, -1, 1, 2, 2, 1, -1, -2 }; //change in row
    int dc[] = { 1, 2, 2, 1, -1, -2, -2, -1 }; //change in col

    for (int i = 0; i < 8; ++i) {
        int destRow = srcRow + dr[i];
        int destCol = srcCol + dc[i];
        if (board.isValidPosition(destRow, destCol)) {
            PieceColor targetPieceColor = board.getPieceColor(destRow, destCol);
            if (targetPieceColor != pieceColor) {
                moves.push_back({ srcRow, srcCol, destRow, destCol,
                    (targetPieceColor == PieceColor::EMPTY) ? MoveType::QUIET : MoveType::CAPTURE });
            }
        }
    }

    return moves;
}

// Function to generate moves for a bishop
std::vector<Move> generateBishopMoves(const Board& board, int srcRow, int srcCol) {
    std::vector<Move> moves;

    //Define the diagonal directions: top-left, top-right, bottom-left, bottom-right
    int dr[] = { -1, -1, 1, 1 };
    int dc[] = { -1, 1, -1, 1 };

    PieceColor pieceColor = board.getPieceColor(srcRow, srcCol);

    //Check moves in each diagonal direction
    for (int i = 0; i < 4; ++i) {
        int destRow = srcRow + dr[i];
        int destCol = srcCol + dc[i];

        //Keep extending the move in the current diagonal direction until we reach the edge of the board or another piece
        while (board.isValidPosition(destRow, destCol)) {
            PieceColor targetPieceColor = board.getPieceColor(destRow, destCol);

            //If the destination square is empty, it's a quiet move
            if (targetPieceColor == PieceColor::EMPTY) {
                moves.push_back({ srcRow, srcCol, destRow, destCol, MoveType::QUIET });
            }
            //If the destination square has an opponent's piece, it's a capture move
            else if (targetPieceColor != pieceColor) {
                moves.push_back({ srcRow, srcCol, destRow, destCol, MoveType::CAPTURE });
                break; //Can't go beyond a captured piece
            }
            else {
                //If the destination square has a piece of the same color, we can't move beyond it
                break;
            }

            //Move further along the diagonal
            destRow += dr[i];
            destCol += dc[i];
        }
    }

    return moves;
}

// Function to generate moves for a queen
std::vector<Move> generateQueenMoves(const Board& board, int srcRow, int srcCol) {
    std::vector<Move> moves;

    //Reuse the logic from the rook and bishop functions

    //Generate rook-like moves (horizontal and vertical)
    std::vector<Move> rookMoves = generateRookMoves(board, srcRow, srcCol);
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());

    //Generate bishop-like moves (diagonal)
    std::vector<Move> bishopMoves = generateBishopMoves(board, srcRow, srcCol);
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());

    return moves;
}

// Function to generate moves for a king
std::vector<Move> generateKingMoves(const Board& board, int srcRow, int srcCol) {
    std::vector<Move> moves;

    PieceColor pieceColor = board.getPieceColor(srcRow, srcCol);

    int dr[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dc[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    for (int i = 0; i < 8; ++i) {
        int destRow = srcRow + dr[i];
        int destCol = srcCol + dc[i];

        if (board.isValidPosition(destRow, destCol)) {
            PieceColor targetPieceColor = board.getPieceColor(destRow, destCol);
            if (targetPieceColor != pieceColor) {
                //Check if the destination square is under attack by the opponent
                //If it's not under attack, it's a valid move for the king
                if (!isSquareAttacked(board, destRow, destCol, getOppositeColor(pieceColor))) {
                    moves.push_back({ srcRow, srcCol, destRow, destCol,
                        (targetPieceColor == PieceColor::EMPTY) ? MoveType::QUIET : MoveType::CAPTURE });
                }
            }
        }
    }

    return moves;
}

PieceColor getOppositeColor(PieceColor color) {
    return (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
}

// Function to check if a square is under attack by an opponent's piece
bool isSquareAttacked(const Board& board, int row, int col, PieceColor attackingColor) {
    int dr[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dc[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    // Check for non-sliding attacks (knights, kings, and pawns)
    for (int i = 0; i < 8; ++i) {
int targetRow = row + dr[i];
int targetCol = col + dc[i];
if (board.isValidPosition(targetRow, targetCol)) {
    PieceColor targetPieceColor = board.getPieceColor(targetRow, targetCol);
    PieceType targetPieceType = board.getPieceType(targetRow, targetCol);
    if (targetPieceColor == attackingColor) {
        if (targetPieceType == PieceType::KNIGHT) {
            if ((std::abs(dr[i]) == 2 && std::abs(dc[i]) == 1) || (std::abs(dr[i]) == 1 && std::abs(dc[i]) == 2)) {
                return true;
            }
        }
        else if (targetPieceType == PieceType::KING) {
            if (std::abs(dr[i]) <= 1 && std::abs(dc[i]) <= 1) {
                return true;
            }
        }
        else if (targetPieceType == PieceType::PAWN) {
            if (attackingColor == PieceColor::WHITE) {
                if (i == 4 && dr[i] == -1 && dc[i] == -1) { // White pawn capturing to the top-left
                    return true;
                }
                else if (i == 5 && dr[i] == -1 && dc[i] == 1) { // White pawn capturing to the top-right
                    return true;
                }
            }
            else {
                if (i == 6 && dr[i] == 1 && dc[i] == -1) { // Black pawn capturing to the bottom-left
                    return true;
                }
                else if (i == 7 && dr[i] == 1 && dc[i] == 1) { // Black pawn capturing to the bottom-right
                    return true;
                }
            }
        }
    }
}
    }

    // Check for sliding attacks (rooks, bishops, and queens)
    int slidingDirections[] = { -1, 1, 0 }; // -1 represents negative direction, 1 represents positive direction, 0 represents no movement
    for (int drDir : slidingDirections) {
        for (int dcDir : slidingDirections) {
            if (drDir == 0 && dcDir == 0) continue; // Skip the case where both directions are 0 (no movement)
            for (int step = 1; step < 8; ++step) {
                int targetRow = row + drDir * step;
                int targetCol = col + dcDir * step;
                if (!board.isValidPosition(targetRow, targetCol)) break; // Stop if the target square is not valid
                PieceColor targetPieceColor = board.getPieceColor(targetRow, targetCol);
                if (targetPieceColor == attackingColor) {
                    PieceType targetPieceType = board.getPieceType(targetRow, targetCol);
                    if (targetPieceType == PieceType::ROOK || targetPieceType == PieceType::QUEEN) {
                        return true;
                    }
                    else {
                        // We reached a piece that is of the same color, no need to check further in this direction
                        break;
                    }
                }
                else if (targetPieceColor != PieceColor::EMPTY) {
                    // We reached a piece of the opponent's color that is not a rook or queen, no need to check further in this direction
                    break;
                }
            }
        }
    }

    return false;
}

//Check if a player is in check
bool isCheck(const Board& board, PieceColor color) {
    int kingRow = -1;
    int kingCol = -1;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board.getPieceType(row, col) == PieceType::KING && board.getPieceColor(row, col) == color) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }
    }

    if (kingRow == -1 || kingCol == -1) {
        //King not found, something's wrong with the board
        return false;
    }
    //Check if the square where the king is located is under attack by the opponent
    return isSquareAttacked(board, kingRow, kingCol, getOppositeColor(color));
}

//Check if a player is in checkmate
bool isCheckmate(const Board& board, PieceColor color) {
    if (!isCheck(board, color)) {
        return false;
    }

    //Check if the player has any legal moves to get out of check
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board.getPieceColor(row, col) == color) {
                std::vector<Move> moves = generateMovesForPiece(board, row, col);
                for (const Move& move : moves) {
                    int srcRow = move.srcRow;
                    int srcCol = move.srcCol;
                    int destRow = move.destRow;
                    int destCol = move.destCol;

                    Board newBoard = board;
                    newBoard.makeMove(srcRow, srcCol, destRow, destCol);
                    if (!isCheck(newBoard, color)) {
                        //Found a legal move to get out of check
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

// Function to generate moves for a piece and apply them to a copy of the board
std::vector<Move> generateMovesForPiece(const Board& board, int row, int col)
{
    PieceType pieceType = board.getPieceType(row, col);
    PieceColor pieceColor = board.getPieceColor(row, col);

    if (pieceType == PieceType::PAWN) {
        return generatePawnMoves(board, row, col);
    }
    else if (pieceType == PieceType::ROOK) {
        return generateRookMoves(board, row, col);
    }
    else if (pieceType == PieceType::KNIGHT) {
        return generateKnightMoves(board, row, col);
    }
    else if (pieceType == PieceType::BISHOP) {
        return generateBishopMoves(board, row, col);
    }
    else if (pieceType == PieceType::QUEEN) {
        return generateQueenMoves(board, row, col);
    }
    else if (pieceType == PieceType::KING) {
        return generateKingMoves(board, row, col);
    }

    // Return an empty vector if the piece type is unknown
    return {};
}

