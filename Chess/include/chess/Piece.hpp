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
	size_t hasMoved_;

public:
	Piece() = delete;
	Piece(Color color);
	virtual ~Piece() = default;

	Piece(const Piece &other) = default;
	Piece &operator=(const Piece &other) = default;
	Piece(Piece &&other) noexcept;
	Piece &operator=(Piece &&other) noexcept;
	size_t getHasMoved() const;
	void addHasMoved();

	void setHasMoved(size_t hasMoved);
	void subtractHasMoved();

	Color getColor() const;
	bool isValidMove(const Position currentPosition, const Position newPosition, const Board &board, CustomMoveType moveType = CustomMoveType::NONE) const;

	virtual PieceType getType() const = 0;
	virtual std::vector<Position> getValidMoves(const Position currentPosition, const Board &board, CustomMoveType moveType = CustomMoveType::NONE) const = 0;
};
