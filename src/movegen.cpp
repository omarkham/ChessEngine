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
                if (board.isSquareAttacked(destRow, destCol, getOppositeColor(pieceColor)) == false) {
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


// Function to generate moves for a piece and apply them to a copy of the board
std::vector<Move> generateMovesForPiece(const Board& board, int row, int col)
{
    PieceType pieceType = board.getPieceType(row, col);

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

std::vector<Move> generateMovesForColor(const Board& board, PieceColor color) {
    std::vector<Move> allMoves;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board.getPieceColor(row, col) == color) {
                std::vector<Move> pieceMoves = generateMovesForPiece(board, row, col);
                allMoves.insert(allMoves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }
    return allMoves;
}
