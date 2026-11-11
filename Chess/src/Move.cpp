#include "chess/Move.hpp"
#include "chess/utils/ChessUtils.hpp"

Move::Move()
	: from_(),
	  to_(),
	  pieceType_(PieceType::NONE),
	  capturedPieceType_(PieceType::NONE),
	  capturedPieceHasMoved_(0),
	  capturedPosition_(),
	  movingPieceHasMoved_(0),
	  previousHalfMoveClock_(0)
{
}

Move::Move(const Position &from, const Position &to)
	: from_(from),
	  to_(to),
	  pieceType_(PieceType::NONE),
	  capturedPieceType_(PieceType::NONE),
	  capturedPieceHasMoved_(0),
	  capturedPosition_(to),
	  movingPieceHasMoved_(0),
	  previousHalfMoveClock_(0)
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
	switch (piece->getType())
	{
	case PieceType::PAWN:
	case PieceType::ROOK:
	case PieceType::KNIGHT:
	case PieceType::BISHOP:
	case PieceType::QUEEN:
	case PieceType::KING:
		pieceType_ = piece->getType();
		break;
	default:
		throw std::invalid_argument("Unknown piece type.");
	}
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

void Move::prepareMove(Board &board, Square *toSquare, Piece *pieceToMove)
{
	Square *captureSquare = customMoveType_ == CustomMoveType::EN_PASSANT
								? &board[from_.getX()][to_.getY()]
								: toSquare;
	const Piece *capturedPiece = captureSquare->getPiece();
	capturedPosition_ = customMoveType_ == CustomMoveType::EN_PASSANT
							? Position(from_.getX(), to_.getY())
							: to_;
	movingPieceHasMoved_ = 0;

	if (capturedPiece != nullptr && capturedPiece->getType() != PieceType::KING)
	{
		capturedPieceType_ = capturedPiece->getType();
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
}

void Move::executeMove(Board &board, Square *fromSquare, Square *toSquare,
					   PieceType promotionType)
{
	Square *captureSquare = customMoveType_ == CustomMoveType::EN_PASSANT
								? &board[from_.getX()][to_.getY()]
								: toSquare;
	std::unique_ptr<Piece> movingPiece = fromSquare->releasePiece();
	std::unique_ptr<Piece> capturedPieceOwner = captureSquare->releasePiece();
	toSquare->setPiece(std::move(movingPiece));

	Square *rookFromSquare = nullptr;
	Square *rookToSquare = nullptr;
	if (customMoveType_ == CustomMoveType::CASTLE_KINGSIDE)
	{
		rookFromSquare = &board[from_.getX()][7];
		rookToSquare = &board[from_.getX()][5];
	}
	else if (customMoveType_ == CustomMoveType::CASTLE_QUEENSIDE)
	{
		rookFromSquare = &board[from_.getX()][0];
		rookToSquare = &board[from_.getX()][3];
	}
	if (customMoveType_ == CustomMoveType::PROMOTION)
	{
		Color color = toSquare->getPiece()->getColor();
		std::unique_ptr<Piece> promotedPiece =
			ChessPieceFactory::createPromotedPiece(promotionType, color);
		if (promotedPiece == nullptr)
			throw InvalidMoveException("Invalid promotion piece.");
		toSquare->setPiece(std::move(promotedPiece));
	}

	if (rookFromSquare != nullptr)
	{
		std::unique_ptr<Piece> rook = rookFromSquare->releasePiece();
		rookToSquare->setPiece(std::move(rook));
	}

	if (ChessAttackUtils::isInCheck(board, toSquare->getPiece()->getColor()))
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
}

void Move::makeMove(Board &board, const Move *lastMove, PieceType promotionType)
{
	Square *fromSquare = &board[from_.getX()][from_.getY()];
	Square *toSquare = &board[to_.getX()][to_.getY()];
	Piece *pieceToMove = fromSquare->getPiece();

	if (pieceToMove != nullptr && pieceToMove->getType() == PieceType::KING &&
		ChessMoveUtils::isCastleKingside(board, from_, to_))
		customMoveType_ = CustomMoveType::CASTLE_KINGSIDE;
	else if (pieceToMove != nullptr && pieceToMove->getType() == PieceType::KING &&
			 ChessMoveUtils::isCastleQueenside(board, from_, to_))
		customMoveType_ = CustomMoveType::CASTLE_QUEENSIDE;
	else if (pieceToMove != nullptr && pieceToMove->getType() == PieceType::PAWN &&
			 ChessMoveUtils::isEnPassant(board, from_, to_, lastMove))
		customMoveType_ = CustomMoveType::EN_PASSANT;
	else if (pieceToMove != nullptr &&
			 ChessMoveUtils::isPromotionMove(board, from_, to_))
		customMoveType_ = CustomMoveType::PROMOTION;

	ChessMoveUtils::validateMove(board, from_, to_, pieceToMove,
								 customMoveType_, promotionType);
	prepareMove(board, toSquare, pieceToMove);
	executeMove(board, fromSquare, toSquare, promotionType);
}

void Move::undoMove(Board &board) const
{
	Square *fromSquare = &board[from_.getX()][from_.getY()];
	Square *toSquare = &board[to_.getX()][to_.getY()];
	std::unique_ptr<Piece> movingPiece = toSquare->releasePiece();
	if (movingPiece == nullptr)
		throw std::runtime_error("Cannot undo a move without a piece on its destination.");

	const bool wasPromotion =
		pieceType_ == PieceType::PAWN && movingPiece->getType() != PieceType::PAWN;
	const bool wasCastling =
		movingPiece->getType() == PieceType::KING &&
		from_.getY() == 4 &&
		(to_.getY() == 2 || to_.getY() == 6);
	if (wasPromotion)
	{
		std::unique_ptr<Piece> pawn =
			ChessPieceFactory::createPiece(PieceType::PAWN, movingPiece->getColor());
		movingPiece = std::move(pawn);
	}
	fromSquare->setPiece(std::move(movingPiece));

	if (wasCastling)
	{
		const int rookFromColumn = to_.getY() == 6 ? 5 : 3;
		const int rookToColumn = to_.getY() == 6 ? 7 : 0;
		std::unique_ptr<Piece> rook =
			board[from_.getX()][rookFromColumn].releasePiece();
		if (rook == nullptr)
			throw std::runtime_error("Cannot undo castling without a rook.");
		board[from_.getX()][rookToColumn].setPiece(std::move(rook));
	}

	if (capturedPieceType_ != PieceType::NONE)
	{
		const Color movingColor = fromSquare->getPiece()->getColor();
		const Color capturedColor = oppositeColor(movingColor);
		std::unique_ptr<Piece> capturedPiece =
			ChessPieceFactory::createPiece(capturedPieceType_, capturedColor);
		board[capturedPosition_.getX()][capturedPosition_.getY()]
			.setPiece(std::move(capturedPiece));
	}
}

CustomMoveType Move::getCustomMoveType() const
{
	return customMoveType_;
}

std::string Move::toString() const
{
	return from_.toString() + " -> " + to_.toString();
}
