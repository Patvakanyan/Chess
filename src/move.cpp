#include "../include/move.hpp"
#include "../include/pawn.hpp"

Move::Move(const Position &from, const Position &to) : from_(from), to_(to), halfMoveClock_(0) {}

const Position &Move::getFrom() const
{
	return from_;
}

const Position &Move::getTo() const
{
	return to_;
}

size_t Move::getHalfMoveClock() const
{
	return halfMoveClock_;
}

void Move::setHalfMoveClock()
{
	halfMoveClock_ = 0;
}

void Move::makeMove(Board &board)
{
	if (!board.isValidPosition(from_) || !board.isValidPosition(to_))
		throw std::invalid_argument("Invalid move position.");

	Square *fromSquare = &board[from_.getX()][from_.getY()];
	Square *toSquare = &board[to_.getX()][to_.getY()];

	Piece *pieceToMove = fromSquare->getPiece();
	if (pieceToMove == nullptr)
		throw std::invalid_argument("No piece at the source position.");

	if (!toSquare->isEmpty() && toSquare->getPiece()->getColor() == pieceToMove->getColor())
		throw std::invalid_argument("Destination is occupied by your own piece.");

	if (!pieceToMove->isValidMove(from_, to_, board))
		throw std::invalid_argument("Invalid move for the selected piece.");

	Piece *capturedPiece = toSquare->getPiece();

	toSquare->setPiece(pieceToMove);
	fromSquare->setPiece(nullptr);

	std::cout << "Color of the piece being moved: " << (pieceToMove->getColor() == Color::White ? "White" : "Black") << std::endl;
	if (ChessUtils::isInCheck(board, pieceToMove->getColor()))
	{
		toSquare->setPiece(capturedPiece);
		fromSquare->setPiece(pieceToMove);
		throw std::invalid_argument("Move puts your king in check.");
	}

	pieceToMove->setHasMoved(true);

	const bool movedPawn = dynamic_cast<Pawn *>(pieceToMove) != nullptr;
	const bool capturedAny = capturedPiece != nullptr;

	if (movedPawn || capturedAny)
	{
		halfMoveClock_ = 0;
	}
	else
	{
		halfMoveClock_++;
	}

	if (capturedPiece != nullptr)
	{
		delete capturedPiece;
	}
}
