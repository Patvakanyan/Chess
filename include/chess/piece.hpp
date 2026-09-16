#pragma once

#include <vector>
#include "chess/position.hpp"
#include "chess/color.hpp"

class Board;

class Piece
{
protected:
	Color color;
	bool hasMoved_;
public:
	Piece() = delete;
	Piece(Color color);
	virtual ~Piece() = default;

	Piece(const Piece &other) = default;
	Piece &operator=(const Piece &other) = default;
	void setHasMoved(bool hasMoved);
	bool getHasMoved() const;

	Color getColor() const;
	bool isValidMove(const Position currentPosition,const Position newPosition, const Board &board) const;

	virtual std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const = 0;
};
