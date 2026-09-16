#pragma once
#include "chess/board.hpp"
#include "chess/position.hpp"
#include "chess/color.hpp"

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
	static Position findKing(const Board &board, Color color);
	static bool isInCheck(const Board &board, Color color);
	static bool isCheckmate(const Board &board, Color color);
	static bool isStalemate(const Board &board, Color color);
};
