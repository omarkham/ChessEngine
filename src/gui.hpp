#ifndef GUI_HPP
#define GUI_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "piece.hpp"
#include "board.hpp"
#include "search.hpp"
#include <SDL_ttf.h>
#include <map>
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <unordered_map>

class GUI {
public:
    GUI();
    ~GUI();
    void run();

private:

    std::unordered_map<PieceColor, PieceType> promotionPieceType;

    PieceColor realPlayerColor = PieceColor::EMPTY;
    PieceColor aiPlayerColor = PieceColor::EMPTY;

    std::map<std::string, SDL_Texture*> cachedTextures;

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

    bool isValidMoveForCurrentPlayer(int srcRow, int srcCol, int destRow, int destCol);
    void printValidMoves(int selectedPieceRow, int selectedPieceCol);
    PieceType choosePromotionPieceType(PieceColor currentPlayerColor);

    Board board;

    SDL_Window* window;
    SDL_Renderer* renderer;
    Piece* selectedPiece;

    void drawChessboard();
    void drawPieces(bool isPieceSelected, int selectedPieceRow, int selectedPieceCol, PieceType promotionPiece = PieceType::EMPTY);
    SDL_Texture* getPieceTexture(PieceType pieceType, PieceColor pieceColor);
    SDL_Texture* loadTexture(const std::string& filePath);

    PieceColor choosePlayerColor();
};

#endif
