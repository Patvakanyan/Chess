#pragma once

#include "chess/square.hpp"
#include "chess/position.hpp"
#include <stdexcept>

#include "pieces/bishop.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"
#include "pieces/pawn.hpp"
#include "pieces/queen.hpp"
#include "pieces/rook.hpp"
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
	Board(Board &&other) = default;
	Board &operator=(Board &&other) = default;
	~Board() = default;
	Square *operator[](size_t index);
	const Square *operator[](size_t index) const;

	void placePiece(Piece *piece, const Position &position);
	Piece *removePiece(const Position &position);
	bool isValidPosition(const Position &position) const;
	void initializeBoard();
};
