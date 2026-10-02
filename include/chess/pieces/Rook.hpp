#pragma once

#include "chess/Piece.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"
class Rook : public Piece
{
public:
	Rook(Color color);
	Rook(const Rook &other) = delete;
	Rook &operator=(const Rook &other) = delete;
	~Rook() = default;

	PieceType getType() const override;
	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board) const override;
};
