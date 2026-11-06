#pragma once

#include "chess/Piece.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"

class Knight : public Piece
{
public:
	explicit Knight(Color color);
	Knight(const Knight &other) = delete;
	Knight &operator=(const Knight &other) = delete;
	~Knight() = default;

	PieceType getType() const override;

protected:
	std::vector<Position> computeValidMoves(const Position currentPosition, const Board &board, CustomMoveType customMoveType) const override;
};
