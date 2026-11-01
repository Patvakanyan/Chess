#pragma once

#include "chess/Piece.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"

class Knight : public Piece
{
public:
	Knight(Color color, size_t hasMoved = 0);
	Knight(const Knight &other) = delete;
	Knight &operator=(const Knight &other) = delete;
	~Knight() = default;

	PieceType getType() const override;
	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board, CustomMoveType moveType = CustomMoveType::NONE) const override;
};
