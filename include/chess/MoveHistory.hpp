#pragma once

#include "chess/Move.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/Piece.hpp"
#include "chess/exceptions/InvalidMove.hpp"
#include "chess/utils/EnumPiece.hpp"
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
	void clear();
	void addMove(const Move &move);
	bool undoMove(Board &board);
	const std::vector<Move> &getMoves() const;
	void printMoveHistory() const;
};
