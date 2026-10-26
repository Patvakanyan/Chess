#include "chess/Piece.hpp"

#include <iostream>
Piece::Piece(Color color) : color(color), hasMoved_(false) {}

Color Piece::getColor() const
{
	return color;
}
bool Piece::isValidMove(const Position currentPosition, const Position newPosition, const Board &board, CustomMoveType moveType) const
{
	if (currentPosition == newPosition)
		return false;

	for (const Position &pos : getValidMoves(currentPosition, board, moveType))
	{
		if (pos == newPosition)
			return true;
	}

	return false;
}

void Piece::addHasMoved()
{
	hasMoved_++;
}

void Piece::subtractHasMoved()
{
	if (hasMoved_ > 0)
		hasMoved_--;
}

size_t Piece::getHasMoved() const
{
	return hasMoved_;
}
