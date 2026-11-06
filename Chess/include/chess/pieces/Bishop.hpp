#pragma once

#include "chess/Piece.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"

class Bishop : public Piece
{
public:
	explicit Bishop(Color color);

	PieceType getType() const override;

protected:
	std::vector<Position> computeValidMoves(const Position currentPosition, const Board &board, CustomMoveType moveType) const override;
};
