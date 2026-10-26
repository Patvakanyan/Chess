#pragma once

#include "chess/Piece.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"

class King : public Piece
{
public:
	King(Color color);
	King(const King &other) = delete;
	King &operator=(const King &other) = delete;
	~King() = default;

	PieceType getType() const override;
	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board, CustomMoveType moveType = CustomMoveType::NONE) const override;

};
