#pragma once

#include "chess/move.hpp"
#include "chess/position.hpp"
#include "chess/board.hpp"
#include "chess/piece.hpp"
#include "chess/exceptions/invalidMove.hpp"
#include "chess/utils/enumPiece.hpp"
#include <vector>

class MoveHistory
{
private:
	std::vector<Move> moves_;
	int halfMoveClock_;
	void setHalfMoveClock();
public:
	MoveHistory();
	MoveHistory(const MoveHistory &other) = delete;
	MoveHistory &operator=(const MoveHistory &other) = delete;
	MoveHistory(MoveHistory &&other) = delete;
	MoveHistory &operator=(MoveHistory &&other) = delete;
	~MoveHistory() = default;

	int getHalfMoveClock() const;
	void addMove(const Move &move);
	bool undoMove(Board &board);
	const std::vector<Move> &getMoves() const;
	void printMoveHistory() const;
};
