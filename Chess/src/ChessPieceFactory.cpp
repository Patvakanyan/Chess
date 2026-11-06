#include "chess/utils/ChessUtils.hpp"

namespace ChessPieceFactory
{

std::unique_ptr<Piece> createPiece(PieceType type, Color color)
{
	switch (type)
	{
	case PieceType::PAWN:
		return std::make_unique<Pawn>(color);
	case PieceType::ROOK:
		return std::make_unique<Rook>(color);
	case PieceType::KNIGHT:
		return std::make_unique<Knight>(color);
	case PieceType::BISHOP:
		return std::make_unique<Bishop>(color);
	case PieceType::QUEEN:
		return std::make_unique<Queen>(color);
	case PieceType::KING:
		return std::make_unique<King>(color);
	default:
		return nullptr;
	}
}

std::unique_ptr<Piece> createPromotedPiece(PieceType type, Color color)
{
	switch (type)
	{
	case PieceType::QUEEN:
		return std::make_unique<Queen>(color);
	case PieceType::ROOK:
		return std::make_unique<Rook>(color);
	case PieceType::BISHOP:
		return std::make_unique<Bishop>(color);
	case PieceType::KNIGHT:
		return std::make_unique<Knight>(color);
	default:
		return nullptr;
	}
}

} // namespace ChessPieceFactory
