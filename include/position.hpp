#pragma once

#include <stdexcept>
#include <string>
class Position
{
private:
	int row_;
	int column_;

public:
	Position(int row = 0, int column = 0);
	Position(const std::string &position);
	int getX() const;
	int getY() const;

	std::string toString() const;
	bool operator==(const Position &other) const;
	bool operator!=(const Position &other) const;
};
