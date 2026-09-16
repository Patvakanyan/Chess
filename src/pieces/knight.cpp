#include "chess/pieces/knight.hpp"

Knight::Knight(Color color) : Piece(color) {}


std::vector <Position> Knight::getValidMoves(const Position currentPosition, const Board &board) const
{
	std::vector<Position> validMoves;
	int currentX = currentPosition.getX();
	int currentY = currentPosition.getY();

	const std::vector<std::pair<int, int>> knightMoves = {
		{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
		{1, 2}, {1, -2}, {-1, 2}, {-1, -2}
	};

	for (const auto &move : knightMoves)
	{
		int newX = currentX + move.first;
		int newY = currentY + move.second;

		if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
		{
			Position newPos(newX, newY);
			const Square &square = board[newPos.getX()][newPos.getY()];

			if (square.isEmpty() || square.getPiece()->getColor() != this->getColor())
			{
				validMoves.push_back(newPos);
			}
		}
	}

	return validMoves;
}
