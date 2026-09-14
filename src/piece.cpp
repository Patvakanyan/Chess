#include "../include/piece.hpp"

Piece::Piece(Color color) : color(color), hasMoved_(false) {}

Color Piece::getColor() const
{
	return color;
}
bool Piece::isValidMove(const Position currentPosition,const Position newPosition, const Board &board) const
{
	if (currentPosition == newPosition)
	{
		return false;
	}
	for (const Position &pos : getValidMoves(currentPosition, board))
	{
//		std::cout << "Vala8id capture position: (" << pos.getX() << ", " << pos.getY() << ")\n";

		if (pos == newPosition)
		{
			return true;
		}
	}

	return false;
}


void Piece::setHasMoved(bool hasMoved)
{
	hasMoved_ = hasMoved;
}
bool Piece::getHasMoved() const
{
	return hasMoved_;
}
