#include "../include/king.hpp"

King::King(Color color) : Piece(color) {}

std::vector<Position> King::getValidMoves(const Position currentPosition, const Board &board) const
{
	std::vector<Position> validMoves;
	int currentX = currentPosition.getX();
	int currentY = currentPosition.getY();

	const std::vector<std::pair<int, int>> directions = {
		{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	for (const auto &dir : directions)
	{
		int x = currentX + dir.first;
		int y = currentY + dir.second;

		if (x < 0 || x >= 8 || y < 0 || y >= 8)
		{
			continue;
		}

		Position newPos(x, y);
		const Square &square = board[newPos.getX()][newPos.getY()];

		if (square.isEmpty())
		{
			validMoves.push_back(newPos);
		}
		else
		{
			if (square.getPiece()->getColor() != this->getColor())
			{
				validMoves.push_back(newPos);
			}
		}
	}
	return validMoves;
}
