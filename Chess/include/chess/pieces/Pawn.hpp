#pragma once

#include "chess/Piece.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"

class Pawn : public Piece
{
public:
	explicit Pawn(Color color);
	Pawn(const Pawn &other) = delete;
	Pawn &operator=(const Pawn &other) = delete;
	~Pawn() = default;

	PieceType getType() const override;

protected:
	std::vector<Position> computeValidMoves(const Position currentPosition, const Board &board, CustomMoveType moveType) const override;
};
