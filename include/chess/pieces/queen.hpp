#pragma once

#include "chess/piece.hpp"
#include "chess/position.hpp"
#include "chess/board.hpp"
#include <vector>

class Queen : public Piece
{
public:
	Queen(Color color);
	Queen(const Queen &other) = delete;
	Queen &operator=(const Queen &other) = delete;
	~Queen() = default;

	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const override;
};
