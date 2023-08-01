#ifndef GUI_HPP
#define GUI_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "piece.hpp"
#include "board.hpp"
#include "mcts.hpp"
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
    // Define wood-like colors
    const Uint32 COLOR_LIGHT = 0xF0D9B5FF; // Light wood color
    const Uint32 COLOR_DARK = 0xB58863FF;  // Dark wood color
    SDL_Color highlightColor = { 255, 255, 128, 100 }; // Light yellow highlight (semi-transparent)
    SDL_Color tintColor = { 218, 165, 32, 100 };       // Golden tint (semi-transparent)


    Board board;
    MCTS ai;

    SDL_Window* window;
    SDL_Renderer* renderer;
    Piece* selectedPiece;

    void drawChessboard();
    void drawPieces(bool isPieceSelected, int selectedPieceRow, int selectedPieceCol);
    SDL_Texture* getPieceTexture(PieceType pieceType, PieceColor pieceColor);
    SDL_Texture* loadTexture(const std::string& filePath);
};

#endif
