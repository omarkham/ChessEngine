#ifndef GUI_HPP
#define GUI_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "piece.hpp"
#include "board.hpp"
#include <SDL_ttf.h>


class GUI {
public:
    GUI();
    ~GUI();
    void run();

private:
    //Constants
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 640;
    const int BOARD_SIZE = 8;
    const int TILE_SIZE = SCREEN_WIDTH / BOARD_SIZE;
    const Uint32 COLOR_LIGHT = 0xEEEEEEFF;
    const Uint32 COLOR_DARK = 0x8CA88AFF;

    Board board;

    SDL_Window* window;
    SDL_Renderer* renderer;
    Piece* selectedPiece;

    void drawChessboard();
    void drawPieces(bool isPieceSelected, int selectedPieceRow, int selectedPieceCol);
    SDL_Texture* getPieceTexture(PieceType pieceType, PieceColor pieceColor);
    SDL_Texture* loadTexture(const std::string& filePath);
};

#endif
