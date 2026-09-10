#pragma once

#include <vector>
#include <stdexcept>


class Position
{
private:
	int x;
	int y;

public:
	Position();
	Position(int x, int y);
	Position(const Position &other);
	Position &operator=(const Position &other) = default;
	~Position() = default;

	int getX() const;
	int getY() const;
	Position operator+(const Position &other) const;
	Position operator-(const Position &other) const;
};
