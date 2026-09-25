#include "chess/move.hpp"

Move::Move(const Position &from, const Position &to) : from_(from), to_(to), pieceType_(PieceType::NONE), capturedPieceType_(PieceType::NONE), capturedPieceHasMoved_(0) {}

const Position &Move::getFrom() const
{
	return from_;
}

const Position &Move::getTo() const
{
	return to_;
}

void Move::setFrom(const Position &from)
{
	from_ = from;
}

void Move::setTo(const Position &to)
{
	to_ = to;
}

void Move::setPieceType(Piece *piece)
{
	if (dynamic_cast<Pawn *>(piece))
		pieceType_ = PieceType::PAWN;
	else if (dynamic_cast<Rook *>(piece))
		pieceType_ = PieceType::ROOK;
	else if (dynamic_cast<Knight *>(piece))
		pieceType_ = PieceType::KNIGHT;
	else if (dynamic_cast<Bishop *>(piece))
		pieceType_ = PieceType::BISHOP;
	else if (dynamic_cast<Queen *>(piece))
		pieceType_ = PieceType::QUEEN;
	else if (dynamic_cast<King *>(piece))
		pieceType_ = PieceType::KING;
	else
		throw std::invalid_argument("Unknown piece type.");
}

PieceType Move::getPieceType() const
{
	return pieceType_;
}

PieceType Move::getCapturedPieceType() const
{
	return capturedPieceType_;
}

size_t Move::getCapturedPieceHasMoved() const
{
	return capturedPieceHasMoved_;
}

void Move::makeMove(Board &board)
{
	if (!board.isValidPosition(from_) || !board.isValidPosition(to_))
		throw InvalidPositionException("Invalid position(s) for the move.");

	Square *fromSquare = &board[from_.getX()][from_.getY()];
	Square *toSquare = &board[to_.getX()][to_.getY()];

	Piece *pieceToMove = fromSquare->getPiece();

	if (!toSquare->isEmpty() && toSquare->getPiece()->getColor() == pieceToMove->getColor())
		throw InvalidMoveException("Destination is occupied by your own piece.");

	if (!pieceToMove->isValidMove(from_, to_, board))
		throw InvalidMoveException("Invalid move for the selected piece.");

	Piece *capturedPiece = toSquare->getPiece();

	if (capturedPiece != nullptr && capturedPiece->getType() != PieceType::KING)
	{
		capturedPieceType_ = capturedPiece->getType();
		capturedPieceHasMoved_ = capturedPiece->getHasMoved();
	}
	else if (capturedPiece != nullptr && capturedPiece->getType() == PieceType::KING)
	{
		throw InvalidMoveException("Cannot capture the king.");
	}
	else
	{
		capturedPieceType_ = PieceType::NONE;
		capturedPieceHasMoved_ = 0;
	}
	capturedPiece = toSquare->releasePiece();
	fromSquare->releasePiece();
	toSquare->setPiece(pieceToMove);

	if (ChessUtils::isInCheck(board, pieceToMove->getColor()))
	{
		toSquare->releasePiece();
		fromSquare->setPiece(pieceToMove);
		toSquare->setPiece(capturedPiece);
		throw InvalidMoveException("Move puts your king in check.");
	}
	pieceToMove->addHasMoved();
	try
	{
		setPieceType(pieceToMove);
	}
	catch (const std::invalid_argument &e)
	{
		throw InvalidMoveException("Unknown piece type.");
	}
	if (capturedPiece != nullptr)
		delete capturedPiece;
}

std::string Move::toString() const
{
	return from_.toString() + " -> " + to_.toString();
}
