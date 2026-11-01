#pragma once

#include "chess/Piece.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"


class Bishop : public Piece
{
public:
	Bishop(Color color, size_t hasMoved = 0);

	PieceType getType() const override;
	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board, CustomMoveType moveType = CustomMoveType::NONE) const override;
};
