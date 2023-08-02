#include "gui.hpp"
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include "search.hpp"
#include <map>

GUI::GUI() : board() {
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
    //Clean up the cached textures
    for (auto& texture : cachedTextures) {
        SDL_DestroyTexture(texture.second);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


// Helper function to check if a move is valid for the current player
bool GUI::isValidMoveForCurrentPlayer(int srcRow, int srcCol, int destRow, int destCol) {
    PieceColor currentPlayerColor = PieceColor::BLACK;
    PieceColor srcPieceColor = board.getPieceColor(srcRow, srcCol);

    if (srcPieceColor == currentPlayerColor && board.isValidMove(srcRow, srcCol, destRow, destCol)) {
        return true;
    }
    return false;
}

void GUI::run() {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    board.initializeFromFEN();

    drawChessboard();

    bool isPieceSelected = false;
    int selectedPieceRow = 0;
    int selectedPieceCol = 0;
    PieceType selectedPieceType = PieceType::EMPTY;
    PieceColor selectedPieceColor = PieceColor::EMPTY;

    SDL_RenderPresent(renderer);

    SDL_Event event;

    bool quit = false;

    // Specify the player's color and AI's color
    PieceColor playerColor = PieceColor::BLACK;
    PieceColor aiColor = PieceColor::WHITE;

    bool isPlayerTurn = false;

    while (!quit && !board.isGameOver()) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseRow = event.button.y / TILE_SIZE;
                int mouseCol = event.button.x / TILE_SIZE;

                if (isPlayerTurn) {
                    // Player's turn
                    if (!isPieceSelected) {
                        // Select a piece
                        PieceType pieceType = board.getPieceType(mouseRow, mouseCol);
                        PieceColor pieceColor = board.getPieceColor(mouseRow, mouseCol);

                        if (pieceType != PieceType::EMPTY && pieceColor == playerColor) {
                            selectedPieceType = pieceType;
                            selectedPieceColor = pieceColor;
                            selectedPieceRow = mouseRow;
                            selectedPieceCol = mouseCol;
                            isPieceSelected = true;
                        }
                    }
                    else {
                        // Check if the player clicked on the same square again to unselect the piece
                        if (selectedPieceRow == mouseRow && selectedPieceCol == mouseCol) {
                            isPieceSelected = false;
                        }
                        else {
                            // Move the selected piece if it is a valid move for the player
                            if (isValidMoveForCurrentPlayer(selectedPieceRow, selectedPieceCol, mouseRow, mouseCol)) {
                                // Create a temporary copy of the board
                                Board tempBoard = board;

                                // Make the move on the temporary board
                                tempBoard.makeMove(selectedPieceRow, selectedPieceCol, mouseRow, mouseCol);

                                // Check if the move leads to the player's own king being in check
                                PieceColor currentPlayerColor = board.getCurrentPlayer();
                                if (!tempBoard.isInCheck()) {
                                    std::cout << "Before Move:" << std::endl;
                                    board.printBoard();
                                    board.makeMove(selectedPieceRow, selectedPieceCol, mouseRow, mouseCol);
                                    std::cout << "After Move:" << std::endl;
                                    board.printBoard();
                                    isPieceSelected = false; // Player's move is complete, deselect the piece

                                    // Change turn after the player makes a valid move
                                    isPlayerTurn = false;
                                }
                                else {
                                    std::cout << "Invalid move! Your own king will be in check." << std::endl;
                                }
                            }
                        }
                    }
                }
            }
        }


        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        drawChessboard();
        drawPieces(isPieceSelected, selectedPieceRow, selectedPieceCol);
        SDL_RenderPresent(renderer);

        if (!isPlayerTurn && !board.isGameOver()) {
            // AI's turn

            // Get the AI's move using alpha-beta search
            Search search;
            Move bestMove = search.alphaBetaSearch(board, 2); // Increase the depth for stronger AI, e.g., 4

            // Make the AI's move
            board.makeMove(bestMove.srcRow, bestMove.srcCol, bestMove.destRow, bestMove.destCol);

            std::cout << "AI Move:" << std::endl;
            board.printBoard();

            // Check if the AI's move resulted in a checkmate
            if (board.isCheckmate()) {
                std::cout << "Checkmate! ";
                if (board.getCurrentPlayer() == PieceColor::WHITE) {
                    std::cout << "Black wins!" << std::endl;
                }
                else {
                    std::cout << "White wins!" << std::endl;
                }
                isPlayerTurn = true; // End the game
                quit = true; // End the game loop
            }
            else if (board.isInCheck()) {
                std::cout << "Check!" << std::endl;

                // Check if the AI's king is in check and try to move it out of check
                if (board.getCurrentPlayer() == aiColor) {
                    Move aiKingMove = search.alphaBetaSearch(board, 2); // Increase the depth for stronger AI, e.g., 4
                    board.makeMove(aiKingMove.srcRow, aiKingMove.srcCol, aiKingMove.destRow, aiKingMove.destCol);
                    std::cout << "AI moves its king." << std::endl;
                }
            }

            isPlayerTurn = true; // AI's move is complete, switch back to the player's turn
        }

    }

    if (board.isGameOver()) {
        // The game is over, you can display a message or perform other actions here
        // For example:
        if (board.isCheckmate()) {
            std::cout << "Checkmate! ";
            if (board.getCurrentPlayer() == playerColor) {
                std::cout << "White wins!" << std::endl;
            }
            else {
                std::cout << "Black wins!" << std::endl;
            }
        }
        else {
            std::cout << "It's a stalemate!" << std::endl;
        }
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

void GUI::drawPieces(bool isPieceSelected, int selectedPieceRow, int selectedPieceCol) {
    // Check for valid moves for the selected piece and apply the light red tint
    if (isPieceSelected) {
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                if (board.isValidMove(selectedPieceRow, selectedPieceCol, row, col)) {
                    SDL_Rect moveRect = {
                        col * TILE_SIZE,
                        row * TILE_SIZE,
                        TILE_SIZE,
                        TILE_SIZE
                    };

                    SDL_SetRenderDrawColor(renderer, tintColor.r, tintColor.g, tintColor.b, tintColor.a);
                    SDL_RenderFillRect(renderer, &moveRect);
                }
            }
        }
    }

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            PieceType pieceType = board.getPieceType(row, col);
            PieceColor pieceColor = board.getPieceColor(row, col);

            if (pieceType != PieceType::EMPTY) {
                // There's a piece on this square

                // Set the position and size of the piece's destination rectangle
                SDL_Rect pieceRect = {
                    col * TILE_SIZE,
                    row * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
                };

                // Highlight the selected piece if it matches the current piece
                if (isPieceSelected && row == selectedPieceRow && col == selectedPieceCol) {
                    SDL_SetRenderDrawColor(renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
                    SDL_RenderFillRect(renderer, &pieceRect);
                }

                // Render the piece texture to the screen
                SDL_RenderCopy(renderer, getPieceTexture(pieceType, pieceColor), nullptr, &pieceRect);
            }
        }
    }
}






SDL_Texture* GUI::getPieceTexture(PieceType pieceType, PieceColor pieceColor) {
    // Define the path to the folder containing the piece textures
    std::string folderPath = "pieces/";

    if (pieceColor == PieceColor::WHITE) {
        switch (pieceType) {
        case PieceType::PAWN:
            return loadTexture(folderPath + "white_pawn.png");
        case PieceType::ROOK:
            return loadTexture(folderPath + "white_rook.png");
            // Add cases for other white piece types (knight, bishop, queen, king) and their corresponding textures
        case PieceType::KNIGHT:
            return loadTexture(folderPath + "white_knight.png");
        case PieceType::BISHOP:
            return loadTexture(folderPath + "white_bishop.png");
        case PieceType::QUEEN:
            return loadTexture(folderPath + "white_queen.png");
        case PieceType::KING:
            return loadTexture(folderPath + "white_king.png");
        default:
            return loadTexture(folderPath + "default_texture.png"); // If no texture is available, use a default texture
        }
    }
    else if (pieceColor == PieceColor::BLACK) {
        // Similar to the white pieces, but with different texture file paths for black pieces.
        switch (pieceType) {
        case PieceType::PAWN:
            return loadTexture(folderPath + "black_pawn.png");
        case PieceType::ROOK:
            return loadTexture(folderPath + "black_rook.png");
            // Add cases for other black piece types (knight, bishop, queen, king) and their corresponding textures
        case PieceType::KNIGHT:
            return loadTexture(folderPath + "black_knight.png");
        case PieceType::BISHOP:
            return loadTexture(folderPath + "black_bishop.png");
        case PieceType::QUEEN:
            return loadTexture(folderPath + "black_queen.png");
        case PieceType::KING:
            return loadTexture(folderPath + "black_king.png");
        default:
            return loadTexture(folderPath + "default_texture.png"); // If no texture is available, use a default texture
        }
    }


    return nullptr; // Return nullptr if the piece color is invalid
}


SDL_Texture* GUI::loadTexture(const std::string& filePath) {
    //Check if the texture is already loaded and cached
    if (cachedTextures.find(filePath) != cachedTextures.end()) {
        return cachedTextures[filePath];
    }

    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (surface == nullptr) {
        // Handle the error if the texture cannot be loaded
        printf("Error loading texture: %s\n", IMG_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr) {
        // Handle the error if the texture cannot be created
        printf("Error creating texture: %s\n", SDL_GetError());
        return nullptr;
    }

    //Cache the loaded texture
    cachedTextures[filePath] = texture;

    return texture;
}
