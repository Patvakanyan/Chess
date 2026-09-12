#pragma once

#include "piece.hpp"
#include "position.hpp"
#include "board.hpp"


class Knight : public Piece
{
public:
	Knight(Color color);
	Knight(const Knight &other) = delete;
	Knight &operator=(const Knight &other) = delete;
	~Knight() = default;

	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const override;
};
