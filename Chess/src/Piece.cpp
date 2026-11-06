#include "chess/Piece.hpp"

Piece::Piece(Color color) : color(color) {}

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

std::vector<Position> Piece::getValidMoves(const Position currentPosition, const Board &board, CustomMoveType moveType) const
{
	return computeValidMoves(currentPosition, board, moveType);
}

Piece::Piece(Piece &&other) noexcept : color(other.color) {}

Piece &Piece::operator=(Piece &&other) noexcept
{
	if (this != &other)
	{
		color = other.color;
	}
	return *this;
}
