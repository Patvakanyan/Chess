#pragma once

#include "chess/Piece.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"

class Rook : public Piece
{
public:
	explicit Rook(Color color);
	Rook(const Rook &other) = delete;
	Rook &operator=(const Rook &other) = delete;
	~Rook() = default;

	PieceType getType() const override;

protected:
	std::vector<Position> computeValidMoves(const Position currentPosition, const Board &board, CustomMoveType moveType) const override;
};
