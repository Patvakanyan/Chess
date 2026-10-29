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

inline char pieceTypeToChar(PieceType pieceType)
{
	switch (pieceType)
	{
	case PieceType::PAWN:
		return 'P';
	case PieceType::ROOK:
		return 'R';
	case PieceType::KNIGHT:
		return 'N';
	case PieceType::BISHOP:
		return 'B';
	case PieceType::QUEEN:
		return 'Q';
	case PieceType::KING:
		return 'K';
	default:
		return ' ';
	}
};

enum class CustomMoveType
{
	CASTLE_KINGSIDE,
	CASTLE_QUEENSIDE,
	EN_PASSANT,
	PROMOTION,
	NONE
};
