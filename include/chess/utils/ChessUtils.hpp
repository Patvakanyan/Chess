#pragma once
#include "chess/Board.hpp"
#include "chess/Position.hpp"
#include "chess/Move.hpp"
#include "chess/utils/Color.hpp"
#include "chess/Move.hpp"

#include <iostream>
#include <string>

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
	static bool canEnPassant(const Board &board, Move lastMove, const Position &from, const Position &to, Color color);

	static bool hasLegalMoves(Board &board, Color color);
	static Position findKing(const Board &board, Color color);
	static bool isInCheck(const Board &board, Color color);
	static bool isCheckmate(Board &board, Color color);
	static bool isStalemate(Board &board, Color color);
	static bool isFiftyMoveRuleDraw(int halfMoveClock);
};
