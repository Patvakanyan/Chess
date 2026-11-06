#include "chess/utils/ChessUtils.hpp"

namespace ChessAttackUtils
{

Position findPiece(const Board &board, Color color, PieceType type)
{
	for (size_t i = 0; i < 8; ++i)
	{
		for (size_t j = 0; j < 8; j++)
		{
			Piece *tmp = board[i][j].getPiece();
			if (tmp && tmp->getColor() == color && tmp->getType() == type)
				return Position(i, j);
		}
	}
	return Position(-1, -1);
}

bool isQueenBishopAttacked(const Board &board, const Position &pos, Color color)
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
				if (piece->getColor() == color && (piece->getType() == PieceType::QUEEN || piece->getType() == PieceType::BISHOP))
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

bool isQueenRookAttacked(const Board &board, const Position &pos, Color color)
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
				if (piece->getColor() == color &&
					(piece->getType() == PieceType::QUEEN || piece->getType() == PieceType::ROOK))
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

bool isKnightAttacked(const Board &board, const Position &pos, Color color)
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
				if (piece && piece->getColor() == color && piece->getType() == PieceType::KNIGHT)
					return true;
			}
		}
	}
	return false;
}

bool isPawnAttacked(const Board &board, const Position &pos, Color color)
{
	// To find attackers of this square, inspect the row opposite the pawn's
	// movement direction.
	int direction = (color == Color::White) ? 1 : -1;
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
				if (piece->getColor() == color && piece->getType() == PieceType::PAWN)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool isKingAttacked(const Board &board, const Position &pos, Color color)
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
				if (piece->getColor() == color && piece->getType() == PieceType::KING)
					return true;
			}
		}
	}
	return false;
}

bool isSquareAttacked(const Board &board, const Position &pos, Color color)
{
	return isQueenRookAttacked(board, pos, color) ||
		   isQueenBishopAttacked(board, pos, color) ||
		   isKnightAttacked(board, pos, color) ||
		   isPawnAttacked(board, pos, color) ||
		   isKingAttacked(board, pos, color);
}

bool isInCheck(const Board &board, Color color)
{
	Position kingPos = findPiece(board, color, PieceType::KING);
	if (kingPos.getX() == -1)
	{
		throw std::runtime_error("King not found on the board.");
	}
	Color opponentColor = (color == Color::White) ? Color::Black : Color::White;
	return isSquareAttacked(board, kingPos, opponentColor);
}

} // namespace ChessAttackUtils
