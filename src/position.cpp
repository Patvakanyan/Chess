#include "../include/position.hpp"

Position::Position(int row, int column)
	: row_(row), column_(column)
{
}

int Position::getX() const
{
	return row_;
}

int Position::getY() const
{
	return column_;
}

bool Position::operator==(const Position &other) const
{
	return row_ == other.row_ && column_ == other.column_;
}

bool Position::operator!=(const Position &other) const
{
	return !(*this == other);
}
