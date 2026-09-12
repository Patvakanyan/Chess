#include "../include/piece.hpp"

Piece::Piece(Color color) : color(color) {}

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
		if (pos == newPosition)
		{
			return true;
		}
	}
	return false;
}


