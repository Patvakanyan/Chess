#pragma once

#include <vector>
#include "chess/position.hpp"
#include "chess/utils/color.hpp"
#include "chess/utils/enumPiece.hpp"

class Board;

class Piece
{
protected:
	Color color;
	size_t hasMoved_;
public:
	Piece() = delete;
	Piece(Color color);
	virtual ~Piece() = default;

	Piece(const Piece &other) = default;
	Piece &operator=(const Piece &other) = default;
	size_t getHasMoved() const;
	void addHasMoved();
	void subtractHasMoved();

	Color getColor() const;
	bool isValidMove(const Position currentPosition,const Position newPosition, const Board &board) const;

	virtual PieceType getType() const = 0;
	virtual std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const = 0;
};
