#include "eval.hpp"
#include "board.hpp"

const int PAWN_VALUE = 100;
const int KNIGHT_VALUE = 300;
const int BISHOP_VALUE = 300;
const int ROOK_VALUE = 500;
const int QUEEN_VALUE = 900;

const int PIECE_POSITIONS[8][8] = {
    {-20, -10, -10, -10, -10, -10, -10, -20},
    {-10,  0,  0,  0,  0,  0,  0, -10},
    {-10,  0,  5, 10, 10,  5,  0, -10},
    {-10,  5,  5, 10, 10,  5,  5, -10},
    {-10,  0, 10, 10, 10, 10,  0, -10},
    {-10, 10, 10, 10, 10, 10, 10, -10},
    {-10,  5,  0,  0,  0,  0,  5, -10},
    {-20, -10, -10, -10, -10, -10, -10, -20}
};

int Evaluation::evaluate(const Board& board, PieceColor color) {
    int whiteScore = 0;
    int blackScore = 0;

    for (int row = 0; row < board.BOARD_SIZE; ++row) {
        for (int col = 0; col < board.BOARD_SIZE; ++col) {
            PieceType pieceType = board.getPieceType(row, col);
            PieceColor pieceColor = board.getPieceColor(row, col);

            int pieceValue = 0;
            switch (pieceType) {
            case PieceType::PAWN:
                pieceValue = PAWN_VALUE;
                break;
            case PieceType::KNIGHT:
                pieceValue = KNIGHT_VALUE;
                break;
            case PieceType::BISHOP:
                pieceValue = BISHOP_VALUE;
                break;
            case PieceType::ROOK:
                pieceValue = ROOK_VALUE;
                break;
            case PieceType::QUEEN:
                pieceValue = QUEEN_VALUE;
                break;
            default:
                // Empty square
                break;
            }

            // Accumulate the score based on piece value and color
            if (pieceColor == PieceColor::WHITE) {
                whiteScore += pieceValue + PIECE_POSITIONS[row][col];
            }
            else if (pieceColor == PieceColor::BLACK) {
                blackScore += pieceValue + PIECE_POSITIONS[7 - row][col]; // Flip the row index for black pieces
            }
        }
    }

    color = board.getAIPlayer();

    return (color == PieceColor::WHITE) ? whiteScore - blackScore : blackScore - whiteScore;
}
