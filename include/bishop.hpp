#pragma once

#include "piece.hpp"
#include "position.hpp"
#include "board.hpp"
#include <cmath>

class Board;

class Bishop : public Piece
{
public:
	Bishop(Color color);

	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const override;
};
