#include "chess/Move.hpp"
#include "chess/utils/ChessUtils.hpp"

Move::Move()
	: from_(),
	  to_(),
	  pieceType_(PieceType::NONE),
	  capturedPieceType_(PieceType::NONE),
	  capturedPieceHasMoved_(0),
	  capturedPosition_(),
	  previousHalfMoveClock_(0)
{
}
Move::Move(const Position &from, const Position &to)
	: from_(from), to_(to), pieceType_(PieceType::NONE),
	  capturedPieceType_(PieceType::NONE), capturedPieceHasMoved_(0),
	  capturedPosition_(to), previousHalfMoveClock_(0)
{
}

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
	if (piece->getType() == PieceType::PAWN)
		pieceType_ = PieceType::PAWN;
	else if (piece->getType() == PieceType::ROOK)
		pieceType_ = PieceType::ROOK;
	else if (piece->getType() == PieceType::KNIGHT)
		pieceType_ = PieceType::KNIGHT;
	else if (piece->getType() == PieceType::BISHOP)
		pieceType_ = PieceType::BISHOP;
	else if (piece->getType() == PieceType::QUEEN)
		pieceType_ = PieceType::QUEEN;
	else if (piece->getType() == PieceType::KING)
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

const Position &Move::getCapturedPosition() const
{
	return capturedPosition_;
}

int Move::getPreviousHalfMoveClock() const
{
	return previousHalfMoveClock_;
}

void Move::setPreviousHalfMoveClock(int halfMoveClock)
{
	previousHalfMoveClock_ = halfMoveClock;
}

#include <iostream>
bool Move::isEnPassant(const Board &board, const Move *lastMove) const
{
	const Square *fromSquare = &board[from_.getX()][from_.getY()];
	const Square *toSquare = &board[to_.getX()][to_.getY()];
	const Square *captureSquare = &board[from_.getX()][to_.getY()];

	if (lastMove == nullptr || fromSquare->isEmpty() || toSquare->getPiece() != nullptr)
		return false;
	if (!captureSquare->isEmpty() && captureSquare->getPiece()->getHasMoved() != 1)
		return false;
	const Piece *movingPiece = fromSquare->getPiece();
	const int direction = movingPiece->getColor() == Color::White ? -1 : 1;
	const int requiredRow = movingPiece->getColor() == Color::White ? 3 : 4;
	if (movingPiece->getType() != PieceType::PAWN ||
		from_.getX() != requiredRow ||
		to_.getX() != from_.getX() + direction ||
		std::abs(to_.getY() - from_.getY()) != 1 ||
		lastMove->getPieceType() != PieceType::PAWN ||
		lastMove->getTo() != Position(from_.getX(), to_.getY()) ||
		lastMove->getFrom().getX() != from_.getX() + 2 * direction ||
		lastMove->getTo().getX() != from_.getX())
		return false;
	const Piece *adjacentPiece = board[from_.getX()][to_.getY()].getPiece();
	return adjacentPiece != nullptr &&
		   adjacentPiece->getType() == PieceType::PAWN &&
		   adjacentPiece->getColor() != movingPiece->getColor();
}

void Move::validateMove(Board &board, Square *toSquare, Piece *pieceToMove,
						bool enPassant) const
{
	if (pieceToMove == nullptr)
		throw EmptySquareException("No piece at the source position.");

	if (!toSquare->isEmpty() && toSquare->getPiece()->getColor() == pieceToMove->getColor())
		throw InvalidMoveException("Destination is occupied by your own piece.");

	if (!enPassant && !pieceToMove->isValidMove(from_, to_, board, CustomMoveType::NONE))
		throw InvalidMoveException("Invalid move for the selected piece.");
	else if (enPassant && !pieceToMove->isValidMove(from_, to_, board, CustomMoveType::EN_PASSANT))
		throw InvalidMoveException("Invalid en passant move for the selected piece.");
}

void Move::applyMove(Board &board, Square *fromSquare, Square *toSquare,
					 Piece *pieceToMove, bool enPassant)
{
	Square *captureSquare = enPassant
								? &board[from_.getX()][to_.getY()]
								: toSquare;
	const Piece *capturedPiece = captureSquare->getPiece();
	capturedPosition_ = enPassant ? Position(from_.getX(), to_.getY()) : to_;

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

	try
	{
		setPieceType(pieceToMove);
	}
	catch (const std::invalid_argument &e)
	{
		throw InvalidMoveException("Unknown piece type.");
	}

	std::unique_ptr<Piece> movingPiece = fromSquare->releasePiece();
	std::unique_ptr<Piece> capturedPieceOwner = captureSquare->releasePiece();
	toSquare->setPiece(std::move(movingPiece));

	if (ChessUtils::isInCheck(board, pieceToMove->getColor()))
	{
		movingPiece = toSquare->releasePiece();
		fromSquare->setPiece(std::move(movingPiece));
		captureSquare->setPiece(std::move(capturedPieceOwner));
		throw InvalidMoveException("Move puts your king in check.");
	}
	pieceToMove->addHasMoved();
}

void Move::makeMove(Board &board, const Move *lastMove)
{
	Square *fromSquare = &board[from_.getX()][from_.getY()];
	Square *toSquare = &board[to_.getX()][to_.getY()];
	Piece *pieceToMove = fromSquare->getPiece();
	const bool enPassant = isEnPassant(board, lastMove);

	validateMove(board, toSquare, pieceToMove, enPassant);
	applyMove(board, fromSquare, toSquare, pieceToMove, enPassant);
}

std::string Move::toString() const
{
	return from_.toString() + " -> " + to_.toString();
}
