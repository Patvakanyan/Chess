#pragma once

#include "chess/Piece.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"

class King : public Piece
{
public:
	explicit King(Color color);
	King(const King &other) = delete;
	King &operator=(const King &other) = delete;
	~King() = default;

	PieceType getType() const override;

protected:
	std::vector<Position> computeValidMoves(const Position currentPosition, const Board &board, CustomMoveType customMoveType) const override;
};
