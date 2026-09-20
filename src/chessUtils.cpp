#include "chess/chessUtils.hpp"

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
bool ChessUtils::isQueenBishopAttacked(const Board &board, const Position &pos, Color color)
{
	const std::vector<Position> directions = {
		Position(-1, -1), Position(-1, 1),
		Position(1, -1), Position(1, 1)};

	for (auto dir : directions)
	{
		int currentX = pos.getX() + dir.getX();
		int currentY = pos.getY() + dir.getY();
		while (currentX >= 0 && currentX < 8 && currentY >= 0 && currentY < 8)
		{
			const Square &square = board[currentX][currentY];
			if (!square.isEmpty())
			{
				Piece *piece = square.getPiece();
				if (piece->getColor() == color && (dynamic_cast<Queen *>(piece) || dynamic_cast<Bishop *>(piece)))
				{
					return true;
				}
				break;
			}
			currentX += dir.getX();
			currentY += dir.getY();
		}
	}
	return false;
}

bool ChessUtils::isQueenRookAttacked(const Board &board, const Position &pos, Color color)
{
	const std::vector<Position> directions = {
		Position(-1, 0), Position(1, 0),
		Position(0, -1), Position(0, 1)};

	for (auto dir : directions)
	{
		int currentX = pos.getX() + dir.getX();
		int currentY = pos.getY() + dir.getY();
		while (currentX >= 0 && currentX < 8 && currentY >= 0 && currentY < 8)
		{
			const Square &square = board[currentX][currentY];

			if (!square.isEmpty())
			{
				Piece *piece = square.getPiece();
				if (piece->getColor() == color && (dynamic_cast<Queen *>(piece) || dynamic_cast<Rook *>(piece)))
				{
					return true;
				}
				break;
			}
			currentX += dir.getX();
			currentY += dir.getY();
		}
	}
	return false;
}

bool ChessUtils::isKnightAttacked(const Board &board, const Position &pos, Color color)
{
	const std::vector<Position> knightMoves = {
		Position(-2, -1), Position(-2, 1),
		Position(-1, -2), Position(-1, 2),
		Position(1, -2), Position(1, 2),
		Position(2, -1), Position(2, 1)};

	for (const auto &move : knightMoves)
	{
		int newX = pos.getX() + move.getX();
		int newY = pos.getY() + move.getY();
		if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
		{
			const Square &square = board[newX][newY];
			if (!square.isEmpty())
			{
				Piece *piece = square.getPiece();
				if (piece && piece->getColor() == color && dynamic_cast<Knight *>(piece))
					return true;
			}
		}
	}
	return false;
}

bool ChessUtils::isPawnAttacked(const Board &board, const Position &pos, Color color)
{
	int direction = (color == Color::White) ? -1 : 1;
	std::vector<Position> pawnAttacks = {
		Position(direction, -1),
		Position(direction, 1)};

	for (const auto &attack : pawnAttacks)
	{
		int newX = pos.getX() + attack.getX();
		int newY = pos.getY() + attack.getY();
		if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
		{
			const Square &square = board[newX][newY];
			if (!square.isEmpty())
			{
				Piece *piece = square.getPiece();
				if (piece->getColor() == color && dynamic_cast<Pawn *>(piece))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool ChessUtils::isKingAttacked(const Board &board, const Position &pos, Color color)
{
	const std::vector<Position> kingMoves = {
		Position(-1, -1), Position(-1, 0), Position(-1, 1),
		Position(0, -1), Position(0, 1),
		Position(1, -1), Position(1, 0), Position(1, 1)};

	for (const auto &move : kingMoves)
	{
		int newX = pos.getX() + move.getX();
		int newY = pos.getY() + move.getY();
		if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
		{
			const Square &square = board[newX][newY];
			if (!square.isEmpty())
			{
				Piece *piece = square.getPiece();
				if (piece->getColor() == color && dynamic_cast<King *>(piece))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool ChessUtils::isSquareAttacked(const Board &board, const Position &pos, Color color)
{
	return isQueenRookAttacked(board, pos, color) ||
		   isQueenBishopAttacked(board, pos, color) ||
		   isKnightAttacked(board, pos, color) ||
		   isPawnAttacked(board, pos, color) ||
		   isKingAttacked(board, pos, color);
}
bool ChessUtils::isInCheck(const Board &board, Color color)
{
	Position kingPos = findKing(board, color);
	if (kingPos.getX() == -1)
	{
		throw std::runtime_error("King not found on the board.");
	}
	Color opponentColor = (color == Color::White) ? Color::Black : Color::White;
	return isSquareAttacked(board, kingPos, opponentColor);
}

bool ChessUtils::hasLegalMoves(Board &board, Color color)
{
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			Piece *piece = board[i][j].getPiece();
			if (piece == nullptr)
				continue;

			if (piece->getColor() != color)
				continue;
			Position currentPos = Position(i, j);
			std::vector<Position> validMoves = piece->getValidMoves(currentPos, board);
			for (const Position &newPos : validMoves)
			{
				Square *fromSquare = &board[currentPos.getX()][currentPos.getY()];
				Square *toSquare = &board[newPos.getX()][newPos.getY()];
				Piece *capturedPiece = toSquare->getPiece();
				toSquare->setPiece(piece);
				fromSquare->setPiece(nullptr);
				piece->addHasMoved();
				bool legal = !isInCheck(board, color);
				toSquare->setPiece(capturedPiece);
				fromSquare->setPiece(piece);
				piece->subtractHasMoved();

				if (legal)
					return true;
			}
		}
	}
	return false;
}

bool ChessUtils::isCheckmate(Board &board, Color color)
{
	return isInCheck(board, color) && !hasLegalMoves(board, color);
}

bool ChessUtils::isStalemate(Board &board, Color color)
{
	return !isInCheck(board, color) && !hasLegalMoves(board, color);
}
