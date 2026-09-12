#pragma once

#include <vector>
#include "position.hpp"
#include "color.hpp"

class Board;

class Piece
{
protected:
	Color color;

public:
	Piece() = delete;
	Piece(Color color);
	virtual ~Piece() = default;

	Piece(const Piece &other) = default;
	Piece &operator=(const Piece &other) = default;

	Color getColor() const;
	bool isValidMove(const Position currentPosition,const Position newPosition, const Board &board) const;

	virtual std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const = 0;
};
