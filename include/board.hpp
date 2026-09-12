#pragma once

#include "./square.hpp"
#include "./position.hpp"
#include <stdexcept>

class Board
{
private:
	Square squares_[8][8];

public:
	Board();
	Board(const Board &other) = delete;
	Board &operator=(const Board &other) = delete;

	Square *operator[](size_t index);
	const Square *operator[](size_t index) const;

	void movePiece(const Position &from, const Position &to);
	void placePiece(Piece *piece, const Position &position);
	Piece *removePiece(const Position &position);
	bool isValidPosition(const Position &position) const;
	void initializeBoard();
};
