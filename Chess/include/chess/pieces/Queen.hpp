#pragma once

#include "chess/Piece.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"
#include <vector>

class Queen : public Piece
{
public:
	explicit Queen(Color color);
	Queen(const Queen &other) = delete;
	Queen &operator=(const Queen &other) = delete;
	~Queen() = default;

	PieceType getType() const override;

protected:
	std::vector<Position> computeValidMoves(const Position currentPosition, const Board &board, CustomMoveType moveType) const override;
};
