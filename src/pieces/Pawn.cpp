#include "chess/pieces/Pawn.hpp"

Pawn::Pawn(Color color) : Piece(color) {}

PieceType Pawn::getType() const
{
	return PieceType::PAWN;
}

std::vector<Position> Pawn::getValidMoves(const Position currentPosition, const Board &board) const
{
	std::vector<Position> validMoves;
	int currentX = currentPosition.getX();
	int currentY = currentPosition.getY();
	Color currentColor = this->getColor();
	int direction = (currentColor == Color::White) ? -1 : 1;
	int startingRow = (currentColor == Color::White) ? 6 : 1;

	int nextRow = currentX + direction;
	if (nextRow >= 0 && nextRow < 8 && board[nextRow][currentY].isEmpty())
		validMoves.emplace_back(nextRow, currentY);
	int twoStepRow = currentX + 2 * direction;
	if (currentX == startingRow && getHasMoved() == 0 && twoStepRow >= 0 && twoStepRow < 8 && board[nextRow][currentY].isEmpty() && board[twoStepRow][currentY].isEmpty())
		validMoves.emplace_back(twoStepRow, currentY);

	for (int dx : {-1, 1})
	{
		int newColumn = currentY + dx;
		int newRow = currentX + direction;
		if (newRow >= 0 && newRow < 8 && newColumn >= 0 && newColumn < 8)
		{
			if (!board[newRow][newColumn].isEmpty() && board[newRow][newColumn].getPiece()->getColor() != currentColor)
				validMoves.emplace_back(newRow, newColumn);

			int enPassantRow = (currentColor == Color::White) ? 3 : 4;
			Piece *adjacentPiece = board[currentX][newColumn].getPiece();
			bool adjacentPawn = adjacentPiece != nullptr &&
								adjacentPiece->getType() == PieceType::PAWN &&
								adjacentPiece->getColor() != currentColor;

			bool immediatelyDoubleStepped = currentX == enPassantRow && adjacentPawn && adjacentPiece->getHasMoved() == 1;
			if (currentX == enPassantRow && board[newRow][newColumn].isEmpty() && immediatelyDoubleStepped)
				validMoves.emplace_back(currentX + direction, newColumn);
		}
	}
	return validMoves;
}
