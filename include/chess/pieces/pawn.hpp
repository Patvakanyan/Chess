#pragma once

#include "chess/piece.hpp"
#include "chess/position.hpp"
#include "chess/board.hpp"
#include "chess/utils/enumPiece.hpp"
#include <cmath>

class Pawn : public Piece
{
public:
	Pawn(Color color);
	Pawn(const Pawn &other) = delete;
	Pawn &operator=(const Pawn &other) = delete;
	~Pawn() = default;

	PieceType getType() const override;
	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const override;
};
