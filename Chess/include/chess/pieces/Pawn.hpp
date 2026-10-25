#pragma once

#include "chess/Piece.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"

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
