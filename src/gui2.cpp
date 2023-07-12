#include "../gui2.hpp"
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

GUI::GUI() {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        window = SDL_CreateWindow("Chess Board", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL) {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
            }
        }
    }
}


GUI::~GUI() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GUI::run() {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    drawChessboard();
    initializePiecesFromFEN("RNBQKBNR/PPPPPPPP/8/8/8/8/pppppppp/rnbqkbnr");//Initialized states
    
    bool isPieceSelected = false;
    int selectedPieceRow = 0;
    int selectedPieceCol = 0;
    
    drawPieces(isPieceSelected, selectedPieceRow, selectedPieceCol);

    SDL_RenderPresent(renderer);

    SDL_Event event;

    bool quit = false;
    Piece* selectedPiece = nullptr;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseRow = event.button.y / TILE_SIZE;
                int mouseCol = event.button.x / TILE_SIZE;

                if (!isPieceSelected) {
                    //Select a piece
                    for (Piece& piece : chessPieces) {
                        if (piece.getRow() == mouseRow && piece.getCol() == mouseCol) {
                            selectedPiece = &piece;
                            selectedPieceRow = mouseRow;
                            selectedPieceCol = mouseCol;
                            isPieceSelected = true;
                            break;
                        }
                    }
                }
                else {
                    //Move the selected piece
                    if (selectedPiece->isValidMove(mouseRow, mouseCol)) {
                        selectedPiece->setRow(mouseRow);
                        selectedPiece->setCol(mouseCol);
                    }

                    selectedPiece = nullptr;
                    isPieceSelected = false;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        drawChessboard();
        drawPieces(isPieceSelected, selectedPieceRow, selectedPieceCol);
        SDL_RenderPresent(renderer);

    }

}

void GUI::drawChessboard() {
    bool isLightTile = true;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            SDL_Rect tileRect = { col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            if (isLightTile) {
                SDL_SetRenderDrawColor(renderer, (COLOR_LIGHT >> 24) & 0xFF, (COLOR_LIGHT >> 16) & 0xFF, (COLOR_LIGHT >> 8) & 0xFF, COLOR_LIGHT & 0xFF);
            }
            else {
                SDL_SetRenderDrawColor(renderer, (COLOR_DARK >> 24) & 0xFF, (COLOR_DARK >> 16) & 0xFF, (COLOR_DARK >> 8) & 0xFF, COLOR_DARK & 0xFF);
            }

            SDL_RenderFillRect(renderer, &tileRect);

            isLightTile = !isLightTile;
        }

        isLightTile = !isLightTile;
    }
}


void GUI::initializePiecesFromFEN(const std::string& fen) {
    //Clear the existing chessPieces vector
    chessPieces.clear();

    //Parse the FEN string
    int row = 7; //Start from the 8th rank(top)
    int col = 0; //Start from the a-file(leftmost)

    for (char c : fen) {
        if (c == '/') {
            //Move to the next rank (row) after encountering '/'
            --row;
            col = 0;
        }
        else if (isdigit(c)) {
            //Skip the specified number of empty squares
            col += (c - '0');
            continue;
        }
        else if (isalpha(c)) {
            //Create a piece object based on the FEN character and add it to the chessPieces vector
            PieceType type;
            PieceColor color;

            switch (c) {
            case 'p':
                type = PieceType::PAWN;
                color = PieceColor::BLACK;
                break;
            case 'r':
                type = PieceType::ROOK;
                color = PieceColor::BLACK;
                break;
            case 'n':
                type = PieceType::KNIGHT;
                color = PieceColor::BLACK;
                break;
            case 'b':
                type = PieceType::BISHOP;
                color = PieceColor::BLACK;
                break;
            case 'q':
                type = PieceType::QUEEN;
                color = PieceColor::BLACK;
                break;
            case 'k':
                type = PieceType::KING;
                color = PieceColor::BLACK;
                break;
            case 'P':
                type = PieceType::PAWN;
                color = PieceColor::WHITE;
                break;
            case 'R':
                type = PieceType::ROOK;
                color = PieceColor::WHITE;
                break;
            case 'N':
                type = PieceType::KNIGHT;
                color = PieceColor::WHITE;
                break;
            case 'B':
                type = PieceType::BISHOP;
                color = PieceColor::WHITE;
                break;
            case 'Q':
                type = PieceType::QUEEN;
                color = PieceColor::WHITE;
                break;
            case 'K':
                type = PieceType::KING;
                color = PieceColor::WHITE;
                break;
            default:
                continue; //Skip any unknown characters
            }

            chessPieces.push_back(Piece(type, color, row, col, renderer));
            ++col;
        }
    }
}

void GUI::drawPieces(bool isPieceSelected, int selectedPieceRow, int selectedPieceCol) {
    for (const Piece& piece : chessPieces) {
        // Set the position and size of the piece's destination rectangle
        SDL_Rect pieceRect = {
            piece.getCol() * TILE_SIZE,
            piece.getRow() * TILE_SIZE,
            TILE_SIZE,
            TILE_SIZE
        };

        // Render the piece texture to the screen
        SDL_RenderCopy(renderer, piece.getTexture(), nullptr, &pieceRect);
    
        //Highlight the selected piece if it matches the current piece
        if (isPieceSelected && piece.getRow() == selectedPieceRow && piece.getCol() == selectedPieceCol) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100); //Red highlight
            SDL_RenderFillRect(renderer, &pieceRect);
        }
    }
}
