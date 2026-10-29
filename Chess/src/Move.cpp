#include "chess/Move.hpp"
#include "chess/utils/ChessUtils.hpp"
#include <iostream>

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

bool Move::isCastleKingside(const Board &board) const
{
	const Square *fromSquare = &board[from_.getX()][from_.getY()];
	const Square *toSquare = &board[to_.getX()][to_.getY()];

	if (fromSquare->isEmpty() || toSquare->getPiece() != nullptr ||
		from_.getX() != to_.getX() || from_.getY() != 4 || to_.getY() != 6)
		return false;

	const Piece *movingPiece = fromSquare->getPiece();
	if (movingPiece->getType() != PieceType::KING || movingPiece->getHasMoved() != 0)
		return false;

	int row = from_.getX();
	if (row != (movingPiece->getColor() == Color::White ? 7 : 0))
		return false;
	const Square *rookSquare = &board[row][7];
	const Piece *rook = rookSquare->getPiece();
	if (rook == nullptr || rook->getType() != PieceType::ROOK ||
		rook->getColor() != movingPiece->getColor() || rook->getHasMoved() != 0)
		return false;

	const Color opponent = movingPiece->getColor() == Color::White ? Color::Black : Color::White;
	for (int col = 4; col <= 6; ++col)
	{
		if ((col != 4 && !board[row][col].isEmpty()) ||
			ChessUtils::isSquareAttacked(board, Position(row, col), opponent))
			return false;
	}
	return true;
}

bool Move::isCastleQueenside(const Board &board) const
{
	const Square *fromSquare = &board[from_.getX()][from_.getY()];
	const Square *toSquare = &board[to_.getX()][to_.getY()];

	if (fromSquare->isEmpty() || toSquare->getPiece() != nullptr ||
		from_.getX() != to_.getX() || from_.getY() != 4 || to_.getY() != 2)
		return false;

	const Piece *movingPiece = fromSquare->getPiece();
	if (movingPiece->getType() != PieceType::KING || movingPiece->getHasMoved() != 0)
		return false;

	int row = from_.getX();
	if (row != (movingPiece->getColor() == Color::White ? 7 : 0))
		return false;
	const Square *rookSquare = &board[row][0];
	const Piece *rook = rookSquare->getPiece();
	if (rook == nullptr || rook->getType() != PieceType::ROOK ||
		rook->getColor() != movingPiece->getColor() || rook->getHasMoved() != 0)
		return false;

	const Color opponent = movingPiece->getColor() == Color::White ? Color::Black : Color::White;
	for (int col = 0; col <= 4; ++col)
	{
		if ((col != 0 && col != 4 && !board[row][col].isEmpty()) ||
			(col >= 2 && ChessUtils::isSquareAttacked(board, Position(row, col), opponent)))
			return false;
	}

	return true;
}

void Move::validateMove(Board &board, Square *toSquare, Piece *pieceToMove,
						CustomMoveType type) const
{
	if (pieceToMove == nullptr)
		throw EmptySquareException("No piece at the source position.");

	if (!toSquare->isEmpty() && toSquare->getPiece()->getColor() == pieceToMove->getColor())
		throw InvalidMoveException("Destination is occupied by your own piece.");
	if (!toSquare->isEmpty() && toSquare->getPiece()->getType() == PieceType::KING)
		throw InvalidMoveException("A king cannot be captured.");

	if (type == CustomMoveType::NONE && !pieceToMove->isValidMove(from_, to_, board, CustomMoveType::NONE))
		throw InvalidMoveException("Invalid move for the selected piece.");
	else if (type == CustomMoveType::EN_PASSANT && !pieceToMove->isValidMove(from_, to_, board, CustomMoveType::EN_PASSANT))
		throw InvalidMoveException("Invalid en passant move for the selected piece.");
	else if (type == CustomMoveType::CASTLE_KINGSIDE && !pieceToMove->isValidMove(from_, to_, board, CustomMoveType::CASTLE_KINGSIDE))
		throw InvalidMoveException("Invalid kingside castling move for the selected piece.");
	else if (type == CustomMoveType::CASTLE_QUEENSIDE && !pieceToMove->isValidMove(from_, to_, board, CustomMoveType::CASTLE_QUEENSIDE))
		throw InvalidMoveException("Invalid queenside castling move for the selected piece.");
}

void Move::applyMove(Board &board, Square *fromSquare, Square *toSquare,
					 Piece *pieceToMove, CustomMoveType type)
{
	Square *captureSquare = type == CustomMoveType::EN_PASSANT
								? &board[from_.getX()][to_.getY()]
								: toSquare;
	const Piece *capturedPiece = captureSquare->getPiece();
	capturedPosition_ = type == CustomMoveType::EN_PASSANT ? Position(from_.getX(), to_.getY()) : to_;

	if (capturedPiece != nullptr && capturedPiece->getType() != PieceType::KING)
	{
		capturedPieceType_ = capturedPiece->getType();
		capturedPieceHasMoved_ = capturedPiece->getHasMoved();
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

	Square *rookFromSquare = nullptr;
	Square *rookToSquare = nullptr;
	if (type == CustomMoveType::CASTLE_KINGSIDE)
	{
		rookFromSquare = &board[from_.getX()][7];
		rookToSquare = &board[from_.getX()][5];
	}
	else if (type == CustomMoveType::CASTLE_QUEENSIDE)
	{
		rookFromSquare = &board[from_.getX()][0];
		rookToSquare = &board[from_.getX()][3];
	}
	if (rookFromSquare != nullptr)
	{
		std::unique_ptr<Piece> rook = rookFromSquare->releasePiece();
		rookToSquare->setPiece(std::move(rook));
		rookToSquare->getPiece()->addHasMoved();
	}

	if (ChessUtils::isInCheck(board, pieceToMove->getColor()))
	{
		if (rookFromSquare != nullptr)
		{
			std::unique_ptr<Piece> rook = rookToSquare->releasePiece();
			rookFromSquare->setPiece(std::move(rook));
		}
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
	CustomMoveType type = CustomMoveType::NONE;

	if (pieceToMove->getType() == PieceType::KING && isCastleKingside(board))
		type = CustomMoveType::CASTLE_KINGSIDE;
	else if (pieceToMove->getType() == PieceType::KING && isCastleQueenside(board))
		type = CustomMoveType::CASTLE_QUEENSIDE;
	else if (pieceToMove->getType() == PieceType::PAWN && isEnPassant(board, lastMove))
		type = CustomMoveType::EN_PASSANT;

	validateMove(board, toSquare, pieceToMove, type);
	applyMove(board, fromSquare, toSquare, pieceToMove, type);
}

void Move::undoMove(Board &board) const
{
	Square *fromSquare = &board[from_.getX()][from_.getY()];
	Square *toSquare = &board[to_.getX()][to_.getY()];
	std::unique_ptr<Piece> movingPiece = toSquare->releasePiece();
	if (movingPiece == nullptr)
		throw std::runtime_error("Cannot undo a move without a piece on its destination.");

	const bool wasCastling =
		movingPiece->getType() == PieceType::KING &&
		from_.getY() == 4 &&
		(to_.getY() == 2 || to_.getY() == 6);
	movingPiece->subtractHasMoved();
	fromSquare->setPiece(std::move(movingPiece));

	if (wasCastling)
	{
		const int rookFromColumn = to_.getY() == 6 ? 5 : 3;
		const int rookToColumn = to_.getY() == 6 ? 7 : 0;
		std::unique_ptr<Piece> rook =
			board[from_.getX()][rookFromColumn].releasePiece();
		if (rook == nullptr)
			throw std::runtime_error("Cannot undo castling without a rook.");
		rook->subtractHasMoved();
		board[from_.getX()][rookToColumn].setPiece(std::move(rook));
	}

	if (capturedPieceType_ != PieceType::NONE)
	{
		const Color movingColor = fromSquare->getPiece()->getColor();
		const Color capturedColor =
			movingColor == Color::White ? Color::Black : Color::White;
		std::unique_ptr<Piece> capturedPiece;
		switch (capturedPieceType_)
		{
		case PieceType::PAWN:
			capturedPiece = std::make_unique<Pawn>(capturedColor);
			break;
		case PieceType::ROOK:
			capturedPiece = std::make_unique<Rook>(capturedColor);
			break;
		case PieceType::KNIGHT:
			capturedPiece = std::make_unique<Knight>(capturedColor);
			break;
		case PieceType::BISHOP:
			capturedPiece = std::make_unique<Bishop>(capturedColor);
			break;
		case PieceType::QUEEN:
			capturedPiece = std::make_unique<Queen>(capturedColor);
			break;
		case PieceType::KING:
			capturedPiece = std::make_unique<King>(capturedColor);
			break;
		default:
			throw std::runtime_error("Cannot restore an unknown captured piece.");
		}

		for (size_t count = 0; count < capturedPieceHasMoved_; ++count)
			capturedPiece->addHasMoved();
		board[capturedPosition_.getX()][capturedPosition_.getY()]
			.setPiece(std::move(capturedPiece));
	}
}

std::string Move::toString() const
{
	return from_.toString() + " -> " + to_.toString();
}
