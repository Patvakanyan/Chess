#include "chess/Piece.hpp"

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

Piece::Piece(Piece &&other) noexcept : color(other.color), hasMoved_(other.hasMoved_)
{
	other.hasMoved_ = 0;
}
void Piece::setHasMoved(size_t hasMoved)
{
	hasMoved_ = hasMoved;
}

Piece &Piece::operator=(Piece &&other) noexcept
{
	if (this != &other)
	{
		color = other.color;
		hasMoved_ = other.hasMoved_;
		other.hasMoved_ = 0;
	}
	return *this;
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
