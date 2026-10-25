#include "chess/Square.hpp"

Square::Square(std::unique_ptr<Piece> piece, Color color)
	: piece_(std::move(piece)), color_(color)
{
}

Piece *Square::getPiece() const
{
	return piece_.get();
}

void Square::setPiece(std::unique_ptr<Piece> piece)
{
	piece_ = std::move(piece);
}

std::unique_ptr<Piece> Square::releasePiece()
{
	return std::move(piece_);
}

bool Square::isEmpty() const
{
	return piece_ == nullptr;
}

void Square::setColor(Color color)
{
	color_ = color;
}
