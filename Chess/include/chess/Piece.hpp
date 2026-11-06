#pragma once

#include <vector>
#include "chess/Position.hpp"
#include "chess/utils/Color.hpp"
#include "chess/utils/EnumPiece.hpp"

class Board;

class Piece
{
protected:
	Color color;

	virtual std::vector<Position> computeValidMoves(const Position currentPosition, const Board &board,
													CustomMoveType moveType) const = 0;

public:
	Piece() = delete;
	Piece(Color color);
	virtual ~Piece() = default;

	Piece(const Piece &other) = default;
	Piece &operator=(const Piece &other) = default;
	Piece(Piece &&other) noexcept;
	Piece &operator=(Piece &&other) noexcept;
	Color getColor() const;
	bool isValidMove(const Position currentPosition, const Position newPosition, const Board &board, CustomMoveType moveType = CustomMoveType::NONE) const;
	std::vector<Position> getValidMoves(const Position currentPosition, const Board &board, CustomMoveType moveType = CustomMoveType::NONE) const;

	virtual PieceType getType() const = 0;
};
