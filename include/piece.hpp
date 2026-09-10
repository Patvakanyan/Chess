#pragma once

#include <vector>
#include "position.hpp"
#include "color.hpp"

class Board;

class Piece
{
protected:
	Color color;
	Position position;

public:
	Piece() = delete;
	Piece(Color color, Position position);
	virtual ~Piece() = default;

	Piece(const Piece &other) = default;
	Piece &operator=(const Piece &other) = default;

	Color getColor() const;
	Position getPosition() const;
	void setPosition(Position newPosition);

	virtual void move(Position newPosition) = 0;

	virtual bool isValidMove(Position newPosition, const Board &board) const = 0;
	virtual std::vector<Position> getValidMoves(const Board &board) const = 0;
};
