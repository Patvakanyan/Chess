#pragma once

#include "chess/piece.hpp"
#include "chess/position.hpp"
#include "chess/board.hpp"


class Knight : public Piece
{
public:
	Knight(Color color);
	Knight(const Knight &other) = delete;
	Knight &operator=(const Knight &other) = delete;
	~Knight() = default;

	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const override;
};
