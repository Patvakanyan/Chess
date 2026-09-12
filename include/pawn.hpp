#pragma once

#include "piece.hpp"
#include "position.hpp"
#include "board.hpp"
#include <cmath>

class Pawn : public Piece
{
private:
	bool hasMoved_;

public:
	Pawn(Color color);
	Pawn(const Pawn &other) = delete;
	Pawn &operator=(const Pawn &other) = delete;
	~Pawn() = default;
	void setHasMoved(bool hasMoved);
	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const override;
};
