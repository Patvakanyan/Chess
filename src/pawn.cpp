#include "../include/pawn.hpp"

Pawn::Pawn(Color color) : Piece(color), hasMoved_(false) {}

std::vector<Position> Pawn::getValidMoves(const Position currentPosition, const Board &board) const
{
	std::vector<Position> validMoves;
	int currentX = currentPosition.getX();
	int currentY = currentPosition.getY();
	int direction = (this->getColor() == Color::White) ? 1 : -1;

	if (currentY + direction >= 0 && currentY + direction < 8 && board[currentX][currentY + direction].isEmpty())
	{
		validMoves.emplace_back(currentX, currentY + direction);
	}

	if (!hasMoved_ && currentY + 2 * direction >= 0 && currentY + 2 * direction < 8 && board[currentX][currentY + 2 * direction].isEmpty() && board[currentX][currentY + direction].isEmpty())
	{
		validMoves.emplace_back(currentX, currentY + 2 * direction);
	}

	for (int dx : {-1, 1})
	{
		int newX = currentX + dx;
		if (newX >= 0 && newX < 8 && currentY + direction >= 0 && currentY + direction < 8)
		{
			if (!board[newX][currentY + direction].isEmpty() && board[newX][currentY + direction].getPiece()->getColor() != getColor())
			{
				validMoves.emplace_back(newX, currentY + direction);
			}
		}
	}
	return validMoves;
}

void Pawn::setHasMoved(bool hasMoved)
{
	hasMoved_ = hasMoved;
}
