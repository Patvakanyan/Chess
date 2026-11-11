#include "chess/utils/ChessUtils.hpp"

namespace ChessAttackUtils
{
namespace
{
	bool isSlidingPieceAttacked(const Board &board, const Position &pos,
								Color color,
								const std::vector<std::pair<int, int>> &directions,
								PieceType secondaryType)
	{
		for (const auto &direction : directions)
		{
			int x = pos.getX() + direction.first;
			int y = pos.getY() + direction.second;
			while (x >= 0 && x < 8 && y >= 0 && y < 8)
			{
				const Square &square = board[x][y];
				if (!square.isEmpty())
				{
					const Piece *piece = square.getPiece();
					if (piece->getColor() == color &&
						(piece->getType() == PieceType::QUEEN ||
						 piece->getType() == secondaryType))
						return true;
					break;
				}
				x += direction.first;
				y += direction.second;
			}
		}
		return false;
	}
}

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
	const std::vector<std::pair<int, int>> offsets = {
		{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
	return isSlidingPieceAttacked(board, pos, color, offsets, PieceType::BISHOP);
}

bool isQueenRookAttacked(const Board &board, const Position &pos, Color color)
{
	const std::vector<std::pair<int, int>> offsets = {
		{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	return isSlidingPieceAttacked(board, pos, color, offsets, PieceType::ROOK);
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
	int direction = color == Color::White ? 1 : -1;
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
	Color opponentColor = oppositeColor(color);
	return isSquareAttacked(board, kingPos, opponentColor);
}

} // namespace ChessAttackUtils
