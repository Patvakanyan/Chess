#pragma once

enum class PieceType
{
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING,
	NONE
};

enum class CustomMoveType
{
	CASTLE_KINGSIDE,
	CASTLE_QUEENSIDE,
	EN_PASSANT,
	PROMOTION,
	NONE
};
