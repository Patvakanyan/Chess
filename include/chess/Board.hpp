#pragma once

#include "chess/Square.hpp"
#include "chess/Position.hpp"
#include <stdexcept>

#include "pieces/Bishop.hpp"
#include "pieces/King.hpp"
#include "pieces/Knight.hpp"
#include "pieces/Pawn.hpp"
#include "pieces/Queen.hpp"
#include "pieces/Rook.hpp"
#include <iostream>

class Board
{
private:
	Square squares_[8][8];
	void printPieceTypes(const Position &position) const;

public:
	Board();
	Board(const Board &other) = delete;
	Board &operator=(const Board &other) = delete;
	Board(Board &&other) = delete;
	Board &operator=(Board &&other) = delete;
	~Board() = default;
	Square *operator[](size_t index);
	const Square *operator[](size_t index) const;

	void placePiece(Piece *piece, const Position &position);
	Piece *removePiece(const Position &position);
	bool isValidPosition(const Position &position) const;
	void initializeBoard();
};
