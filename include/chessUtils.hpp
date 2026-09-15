#pragma once
#include "board.hpp"
#include "position.hpp"
#include "color.hpp"

class ChessUtils
{
public:
	ChessUtils() = delete;

	static bool isSquareAttacked(const Board &board, const Position &pos, Color color);

	static Position findKing(const Board &board, Color color);
	static bool isInCheck(const Board &board, Color color);
	static bool isCheckmate(const Board &board, Color color);
	static bool isStalemate(const Board &board, Color color);
};
