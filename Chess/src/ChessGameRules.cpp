#include "chess/utils/ChessUtils.hpp"

namespace ChessGameRules
{

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
			std::vector<Position> validMoves = piece->getValidMoves(currentPos, board);
			for (const Position &newPos : validMoves)
			{
				Move candidate(currentPos, newPos);
				const PieceType promotionType =
					piece->getType() == PieceType::PAWN &&
							(newPos.getX() == 0 || newPos.getX() == 7)
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

				const bool legal = !ChessAttackUtils::isInCheck(board, color);
				candidate.undoMove(board);

				if (legal)
					return true;
			}
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
