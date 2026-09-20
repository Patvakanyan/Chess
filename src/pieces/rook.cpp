#include "chess/pieces/rook.hpp"

Rook::Rook(Color color) : Piece(color) {}



std::vector<Position> Rook::getValidMoves(const Position currentPosition, const Board &board) const
{
	std::vector<Position> validMoves;

	const std::vector<std::pair<int, int>> directions = {
		{1, 0}, {-1, 0}, {0, 1}, {0, -1}
	};

	for (const auto &dir : directions)
	{
		int x = currentPosition.getX();
		int y = currentPosition.getY();

		while (true)
		{
			x += dir.first;
			y += dir.second;

			if (x < 0 || x >= 8 || y < 0 || y >= 8)
				break;

			Position newPos(x, y);
			const Square &square = board[newPos.getX()][newPos.getY()];

			if (square.isEmpty())
				validMoves.push_back(newPos);
			else
			{
				if (square.getPiece()->getColor() != this->getColor())
				{
					validMoves.push_back(newPos);
				}
				break;
			}
		}
	}
	return validMoves;
}
