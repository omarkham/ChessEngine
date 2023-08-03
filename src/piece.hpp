
#ifndef PIECE_HPP
#define PIECE_HPP

#include <SDL.h>
#include <vector>
#include <string>
#include <SDL_ttf.h>

enum class PieceType {
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING,
	EMPTY
};

enum class PieceColor {
	WHITE,
	BLACK,
	EMPTY
};

class Piece {
public:
	Piece(PieceType type, PieceColor color, int initialRow, int initialCol, SDL_Renderer* renderer);
	//Getters and setters for position, type, color, etc
	void setRow(int newRow);
	void setCol(int newCol);
	SDL_Texture* getTexture() const;
	int getCol() const;
	int getRow() const;
private:
	SDL_Texture* texture;
	int col;
	int row;
};

#endif //PIECE_HPP