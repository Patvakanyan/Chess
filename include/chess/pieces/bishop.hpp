#pragma once

#include "chess/piece.hpp"
#include "chess/position.hpp"
#include "chess/board.hpp"
#include "chess/chessUtils.hpp"
#include <cmath>

class Board;

class Bishop : public Piece
{
public:
	Bishop(Color color);

	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const override;
};
