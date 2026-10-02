#pragma once

#include "chess/Piece.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"
#include <vector>

class Queen : public Piece
{
public:
	Queen(Color color);
	Queen(const Queen &other) = delete;
	Queen &operator=(const Queen &other) = delete;
	~Queen() = default;

	PieceType getType() const override;
	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const override;
};
