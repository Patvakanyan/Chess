#pragma once

#include "piece.hpp"

class King : public Piece
{
public:
	King(Color color, Position position);
	King(const King &other) = default;
	King &operator=(const King &other) = default;
	~King() = default;
	
};
