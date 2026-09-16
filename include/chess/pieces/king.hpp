#pragma once

#include "chess/piece.hpp"
#include "chess/position.hpp"
#include "chess/board.hpp"

class King : public Piece
{
public:
	King(Color color);
	King(const King &other) = delete;
	King &operator=(const King &other) = delete;
	~King() = default;

	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const override;

};
