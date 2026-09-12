#pragma once

class Position
{
private:
	int row_;
	int column_;

public:
	Position(int row = 0, int column = 0);

	int getX() const;
	int getY() const;

	bool operator==(const Position &other) const;
	bool operator!=(const Position &other) const;
};
