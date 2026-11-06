#pragma once

#include "chess/Position.hpp"
#include "chess/utils/Color.hpp"
#include <optional>

struct CastlingRights
{
	bool whiteKingside = true;
	bool whiteQueenside = true;
	bool blackKingside = true;
	bool blackQueenside = true;
};

struct GameState
{
	Color turn = Color::White;
	CastlingRights castling;
	std::optional<Position> enPassantTarget;
	int halfMoveClock = 0;
	int fullMoveNumber = 1;
};
