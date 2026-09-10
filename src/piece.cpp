#include "../include/piece.hpp"

Piece::Piece(Color color, Position position) : color(color), position(position) {}

Color Piece::getColor() const
{
	return color;
}

Position Piece::getPosition() const
{
	return position;
}

void Piece::setPosition(Position newPosition)
{
	position = newPosition;
}
