#include "piece.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>

Piece::Piece(PieceType type, PieceColor color, int initialRow, int initialCol, SDL_Renderer* renderer) : col(initialCol), row(initialRow), texture(nullptr) {
    // Set the path to the image asset based on the piece type and color

    std::string imagePath;
    if (color == PieceColor::BLACK) {
        switch (type) {
        case PieceType::PAWN:
            imagePath = "pieces/black_pawn.png";
            break;
        case PieceType::ROOK:
            imagePath = "pieces/black_rook.png";
            break;
        case PieceType::KNIGHT:
            imagePath = "pieces/black_knight.png";
            break;
        case PieceType::BISHOP:
            imagePath = "pieces/black_bishop.png";
            break;
        case PieceType::QUEEN:
            imagePath = "pieces/black_queen.png";
            break;
        case PieceType::KING:
            imagePath = "pieces/black_king.png";
            break;
        }
    }
    else {
        switch (type) {
        case PieceType::PAWN:
            imagePath = "pieces/white_pawn.png";
            break;
        case PieceType::ROOK:
            imagePath = "pieces/white_rook.png";
            break;
        case PieceType::KNIGHT:
            imagePath = "pieces/white_knight.png";
            break;
        case PieceType::BISHOP:
            imagePath = "pieces/white_bishop.png";
            break;
        case PieceType::QUEEN:
            imagePath = "pieces/white_queen.png";
            break;
        case PieceType::KING:
            imagePath = "pieces/white_king.png";
            break;
        }
    }

    //Set the initial column and row values
    //col = initialCol;
    //row = initialRow;

    // Load the image asset and create a texture
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (surface == nullptr) {
        printf("Failed to load image %s! SDL_image Error: %s\n", imagePath.c_str(), IMG_GetError());
        texture = nullptr;
    }
    else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr) {
            printf("Failed to create texture from surface! SDL Error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(surface);
    }
}

SDL_Texture* Piece::getTexture() const {
    return texture;
}


int Piece::getCol() const {
    return col;
}

int Piece::getRow() const {
    return row;
}

void Piece::setRow(int newRow) {
    row = newRow;
}

void Piece::setCol(int newCol) {
    col = newCol;
}
