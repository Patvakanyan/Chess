#include "../include/position.hpp"

Position::Position() : x(0), y(0) {}

Position::Position(int x, int y) {
	if (x < 0 || x > 7 || y < 0 || y > 7) {
		throw std::out_of_range("Position coordinates must be between 0 and 7.");
	}
	this->x = x;
	this->y = y;
}

int Position::getX() const
{
	return x;
}

int Position::getY() const
{
	return y;
}

Position Position::operator+(const Position &other) const
{
	return Position(x + other.x, y + other.y);
}

Position Position::operator-(const Position &other) const
{
	return Position(x - other.x, y - other.y);
}
