#include "../include/chessUtils.hpp"

Position ChessUtils::findKing(const Board &board, Color color)
{
	for (size_t i = 0; i < 8; ++i)
	{
		for (size_t j = 0; j < 8; j++)
		{
			Piece *tmp = board[i][j].getPiece();
			if (dynamic_cast<King *>(tmp) && tmp->getColor() == color)
				return Position(i, j);
		}
	}
	return Position(-1, -1);
}

bool ChessUtils::isSquareAttacked(const Board &board, const Position &pos, Color color)
{
	const std::vector<Position> directions = {
		Position(-1, -1), Position(-1, 0), Position(-1, 1),
		Position(0, -1), Position(0, 1),
		Position(1, -1), Position(1, 0), Position(1, 1)};

	for (auto dir : directions)
	{
		int currentX = pos.getX();
		int currentY = pos.getY();
		while (currentX >= 0 && currentX < 8 && currentY >= 0 && currentY < 8)
		{
			const Square &square = board[currentX][currentY];
			if (!square.isEmpty())
			{
				Piece *piece = square.getPiece();
				if (piece->getColor() != color)
				{
					if (dynamic_cast<Queen *>(piece) || dynamic_cast<Rook *>(piece) || dynamic_cast<Bishop *>(piece) || dynamic_cast<Knight *>(piece) || dynamic_cast<Pawn *>(piece))
					{
						return true;
					}
				}
				break;
			}
			currentX += dir.getX();
			currentY += dir.getY();
		}
	}
	return false;
}
bool ChessUtils::isInCheck(const Board &board, Color color)
{
	Position kingPos = findKing(board, color);
	if (kingPos.getX() == -1)
	{
		throw std::runtime_error("King not found on the board.");
	}
	return isSquareAttacked(board, kingPos, color);
}
