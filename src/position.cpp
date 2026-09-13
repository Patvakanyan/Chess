#include <iostream>

#include "../include/position.hpp"

Position::Position(int row, int column)
	: row_(row), column_(column)
{
}

Position::Position(const std::string &position)
{
	if (position.length() != 2 || position[0] < 'a' || position[0] > 'h' || position[1] < '1' || position[1] > '8')
	{
		throw std::invalid_argument("Invalid position string");
	}

	column_ = position[0] - 'a';
	row_ = 8 - (position[1] - '0');
	std::cout << "Position created: (" << row_ << ", " << column_ << ")" << std::endl;
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


