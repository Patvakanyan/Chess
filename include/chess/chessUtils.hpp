#pragma once
#include "chess/board.hpp"
#include "chess/position.hpp"
#include "chess/color.hpp"
#include "chess/move.hpp"

class ChessUtils
{
public:
	ChessUtils() = delete;

	static bool isSquareAttacked(const Board &board, const Position &pos, Color color);
	static bool isQueenRookAttacked(const Board &board, const Position &pos, Color color);
	static bool isQueenBishopAttacked(const Board &board, const Position &pos, Color color);
	static bool isKnightAttacked(const Board &board, const Position &pos, Color color);
	static bool isPawnAttacked(const Board &board, const Position &pos, Color color);
	static bool isKingAttacked(const Board &board, const Position &pos, Color color);

	static bool hasLegalMoves(Board &board, Color color);
	static Position findKing(const Board &board, Color color);
	static bool isInCheck(const Board &board, Color color);
	static bool isCheckmate(Board &board, Color color);
	static bool isStalemate(Board &board, Color color);
};
