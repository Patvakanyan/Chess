#include "chess/square.hpp"

Square::Square(Piece *piece, Color color) : piece_(piece), color_(color)
{
}

Square::~Square()
{
	if (piece_ != nullptr)
	{
		delete piece_;
		piece_ = nullptr;
	}
}

Piece *Square::getPiece() const
{
	return piece_;
}

void Square::setPiece(Piece *piece)
{
	if (piece_ != piece)
		delete piece_;
	piece_ = piece;
}

Piece *Square::releasePiece()
{
	Piece *piece = piece_;
	piece_ = nullptr;
	return piece;
}

bool Square::isEmpty() const
{
	return piece_ == nullptr;
}

void Square::setColor(Color color)
{
	color_ = color;
}
