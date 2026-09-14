#include "../include/moveHistory.hpp"

const std::vector<Move> &MoveHistory::getMoves() const
{
	return moves_;
}


void MoveHistory::addMove(const Move &move)
{
	moves_.push_back(move);
}
void MoveHistory::printMoveHistory() const
{
	if (moves_.empty())
	{
		std::cout << "No moves have been made yet." << std::endl;
		return;
	}

	std::cout << "Move History:" << std::endl;
	for (size_t i = 0; i < moves_.size(); ++i)
	{
		const Move &move = moves_[i];
		std::cout << i + 1 << ". " << move.getFrom().toString() << " -> " << move.getTo().toString() << std::endl;
	}
}
