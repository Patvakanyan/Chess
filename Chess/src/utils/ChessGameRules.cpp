#include "chess/utils/ChessUtils.hpp"

#include <algorithm>

namespace ChessGameRules
{

std::vector<Position> legalMovesFrom(Board &board, const Position &from,
									 const Move *lastMove,
									 bool canCastleQueenside,
									 bool canCastleKingside)
{
	std::vector<Position> candidates;
	Piece *piece = board[from.getX()][from.getY()].getPiece();
	if (piece == nullptr)
		return candidates;

	candidates = piece->getValidMoves(from, board);
	if (piece->getType() == PieceType::KING)
	{
		if (canCastleQueenside)
			candidates.emplace_back(from.getX(), 2);
		if (canCastleKingside)
			candidates.emplace_back(from.getX(), 6);
	}
	if (piece->getType() == PieceType::PAWN)
	{
		const std::vector<Position> enPassantMoves =
			piece->getValidMoves(from, board, CustomMoveType::EN_PASSANT);
		candidates.insert(candidates.end(), enPassantMoves.begin(),
						  enPassantMoves.end());
	}

	std::vector<Position> legal;
	for (const Position &to : candidates)
	{
		if (std::find(legal.begin(), legal.end(), to) != legal.end())
			continue;

		Move candidate(from, to);
		const PieceType promotionType =
			ChessMoveUtils::isPromotionMove(board, from, to)
				? PieceType::QUEEN
				: PieceType::NONE;
		try
		{
			candidate.makeMove(board, lastMove, promotionType);
		}
		catch (const InvalidMoveException &)
		{
			continue;
		}

		candidate.undoMove(board);
		legal.push_back(to);
	}
	return legal;
}

bool hasLegalMoves(Board &board, Color color, const Move *lastMove)
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
			if (!legalMovesFrom(board, currentPos, lastMove).empty())
				return true;
		}
	}
	return false;
}

bool isCheckmate(Board &board, Color color, const Move *lastMove)
{
	return ChessAttackUtils::isInCheck(board, color) && !hasLegalMoves(board, color, lastMove);
}

bool isStalemate(Board &board, Color color, const Move *lastMove)
{
	return !ChessAttackUtils::isInCheck(board, color) && !hasLegalMoves(board, color, lastMove);
}

bool isFiftyMoveRuleDraw(int halfMoveClock)
{
	return halfMoveClock >= 100;
}

} // namespace ChessGameRules
