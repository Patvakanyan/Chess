#pragma once

#include "chess/move.hpp"
#include <vector>

class MoveHistory
{
private:
	std::vector<Move> moves_;
public:
	MoveHistory() = default;
	MoveHistory(const MoveHistory &other) = delete;
	MoveHistory &operator=(const MoveHistory &other) = delete;
	~MoveHistory() = default;
	void addMove(const Move &move);
	const std::vector<Move> &getMoves() const;
	void printMoveHistory() const;
};
