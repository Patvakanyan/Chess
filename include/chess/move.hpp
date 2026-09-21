#pragma once

#include "chess/position.hpp"
#include "chess/board.hpp"
#include "chess/chessUtils.hpp"
#include "chess/exceptions/invalidMove.hpp"
#include "chess/exceptions/invalidPositionException.hpp"
#include "chess/exceptions/emptySquareException.hpp"

class Move
{
private:
	Position from_;
	Position to_;
	size_t halfMoveClock_;
	void setHalfMoveClock();

public:
	Move() = default;
	Move(const Position &from, const Position &to);
	void makeMove(Board &board);
	const Position &getFrom() const;
	const Position &getTo() const;

	std::string toString() const;
	void setFrom(const Position &from);
	void setTo(const Position &to);
	size_t getHalfMoveClock() const;
};
