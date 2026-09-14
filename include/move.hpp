#pragma once

#include "position.hpp"
#include "board.hpp"

class Move
{
private:
	Position from_;
	Position to_;
	size_t halfMoveClock_;
public:
	Move(const Position &from, const Position &to);
	void makeMove(Board &board);
	const Position &getFrom() const;
	const Position &getTo() const;

	size_t getHalfMoveClock() const;
	void setHalfMoveClock();
};
