#include "chess/utils/ChessUtils.hpp"

namespace ChessMoveUtils
{
namespace
{
	bool isCastle(const Board &board, const Position &from, const Position &to,
				 bool kingside)
	{
		const int destinationColumn = kingside ? 6 : 2;
		const int rookColumn = kingside ? 7 : 0;
		if (board[from.getX()][from.getY()].isEmpty() ||
			!board[to.getX()][to.getY()].isEmpty() ||
			from.getX() != to.getX() || from.getY() != 4 ||
			to.getY() != destinationColumn)
			return false;

		const Piece *king = board[from.getX()][from.getY()].getPiece();
		const int row = from.getX();
		const int homeRow = king->getColor() == Color::White ? 7 : 0;
		const Piece *rook = board[row][rookColumn].getPiece();
		if (king->getType() != PieceType::KING || row != homeRow ||
			rook == nullptr || rook->getType() != PieceType::ROOK ||
			rook->getColor() != king->getColor())
			return false;

		const Color opponent = oppositeColor(king->getColor());
		if (kingside)
		{
			for (int column = 4; column <= 6; ++column)
			{
				if ((column != 4 && !board[row][column].isEmpty()) ||
					ChessAttackUtils::isSquareAttacked(
						board, Position(row, column), opponent))
					return false;
			}
		}
		else
		{
			for (int column = 0; column <= 4; ++column)
			{
				if ((column != 0 && column != 4 && !board[row][column].isEmpty()) ||
					(column >= 2 && ChessAttackUtils::isSquareAttacked(
									 board, Position(row, column), opponent)))
					return false;
			}
		}
		return true;
	}
}

bool isPromotionMove(const Board &board, const Position &from, const Position &to)
{
	const Square *fromSquare = &board[from.getX()][from.getY()];
	if (fromSquare->isEmpty())
		return false;

	const Piece *movingPiece = fromSquare->getPiece();
	if (movingPiece->getType() != PieceType::PAWN)
		return false;

	const int promotionRow = movingPiece->getColor() == Color::White ? 0 : 7;
	return to.getX() == promotionRow;
}

bool isEnPassant(const Board &board, const Position &from, const Position &to,
				const Move *lastMove)
{
	if (lastMove == nullptr || board[from.getX()][from.getY()].isEmpty() ||
		!board[to.getX()][to.getY()].isEmpty())
		return false;

	const Piece *movingPiece = board[from.getX()][from.getY()].getPiece();
	const int direction = movingPiece->getColor() == Color::White ? -1 : 1;
	const int requiredRow = movingPiece->getColor() == Color::White ? 3 : 4;
	if (movingPiece->getType() != PieceType::PAWN ||
		from.getX() != requiredRow ||
		to.getX() != from.getX() + direction ||
		std::abs(to.getY() - from.getY()) != 1 ||
		lastMove->getPieceType() != PieceType::PAWN ||
		lastMove->getTo() != Position(from.getX(), to.getY()) ||
		lastMove->getFrom().getX() != from.getX() + 2 * direction ||
		lastMove->getTo().getX() != from.getX())
		return false;

	const Piece *adjacentPiece = board[from.getX()][to.getY()].getPiece();
	return adjacentPiece != nullptr &&
		   adjacentPiece->getType() == PieceType::PAWN &&
		   adjacentPiece->getColor() != movingPiece->getColor();
}

void validateMove(const Board &board, const Position &from, const Position &to,
				  const Piece *pieceToMove, CustomMoveType moveType,
				  PieceType promotionType)
{
	const Square *toSquare = &board[to.getX()][to.getY()];
	if (pieceToMove == nullptr)
		throw EmptySquareException("No piece at the source position.");
	if (!toSquare->isEmpty() &&
		toSquare->getPiece()->getColor() == pieceToMove->getColor())
		throw InvalidMoveException("Destination is occupied by your own piece.");
	if (!toSquare->isEmpty() && toSquare->getPiece()->getType() == PieceType::KING)
		throw InvalidMoveException("A king cannot be captured.");

	if (!pieceToMove->isValidMove(from, to, board, moveType))
	{
		switch (moveType)
		{
		case CustomMoveType::EN_PASSANT:
			throw InvalidMoveException("Invalid en passant move for the selected piece.");
		case CustomMoveType::CASTLE_KINGSIDE:
			throw InvalidMoveException("Invalid kingside castling move for the selected piece.");
		case CustomMoveType::CASTLE_QUEENSIDE:
			throw InvalidMoveException("Invalid queenside castling move for the selected piece.");
		case CustomMoveType::PROMOTION:
			throw InvalidMoveException("Invalid promotion move for the selected piece.");
		default:
			throw InvalidMoveException("Invalid move for the selected piece.");
		}
	}

	if (moveType == CustomMoveType::PROMOTION &&
		ChessPieceFactory::createPromotedPiece(promotionType, pieceToMove->getColor()) == nullptr)
		throw InvalidMoveException("Invalid promotion piece.");
}

bool isCastleKingside(const Board &board, const Position &from, const Position &to)
{
	return isCastle(board, from, to, true);
}

bool isCastleQueenside(const Board &board, const Position &from, const Position &to)
{
	return isCastle(board, from, to, false);
}

} // namespace ChessMoveUtils
