#include "chess/pieces/King.hpp"
#include <iostream>
King::King(Color color) : Piece(color) {}

PieceType King::getType() const
{
	return PieceType::KING;
}

std::vector<Position> King::getValidMoves(const Position currentPosition, const Board &board, CustomMoveType type) const
{
	std::vector<Position> validMoves;
	int currentX = currentPosition.getX();
	int currentY = currentPosition.getY();

	const std::vector<std::pair<int, int>> directions = {
		{1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}};

	for (const auto &dir : directions)
	{
		int x = currentX + dir.first;
		int y = currentY + dir.second;

		if (x < 0 || x >= 8 || y < 0 || y >= 8)
			continue;
		Position newPos(x, y);
		const Square &square = board[newPos.getX()][newPos.getY()];
		if (square.isEmpty())
		{
			validMoves.push_back(newPos);
		}
		else
		{
			if (square.getPiece()->getColor() != this->getColor())
				validMoves.push_back(newPos);
		}
	}
	if (type == CustomMoveType::CASTLE_KINGSIDE || type == CustomMoveType::CASTLE_QUEENSIDE)
	{
		int row = currentX;
		if (type == CustomMoveType::CASTLE_KINGSIDE)
		{
			validMoves.push_back(Position(row, currentY + 2));
		}
		else if (type == CustomMoveType::CASTLE_QUEENSIDE)
		{
			validMoves.push_back(Position(row, currentY - 2));
		}
	}

	return validMoves;
}
