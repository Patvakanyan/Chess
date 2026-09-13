#pragma once

#include "piece.hpp"
#include "position.hpp"
#include "board.hpp"
#include <cmath>

class Pawn : public Piece
{

public:
	Pawn(Color color);
	Pawn(const Pawn &other) = delete;
	Pawn &operator=(const Pawn &other) = delete;
	~Pawn() = default;
	
	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const override;
};
