#pragma once

#include "chess/piece.hpp"
#include "chess/position.hpp"
#include "chess/board.hpp"
class Rook : public Piece
{
public:
	Rook(Color color);
	Rook(const Rook &other) = delete;
	Rook &operator=(const Rook &other) = delete;
	~Rook() = default;

	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const override;
};
