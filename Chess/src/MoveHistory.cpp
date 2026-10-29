#include "chess/MoveHistory.hpp"

MoveHistory::MoveHistory() : halfMoveClock_(0) {}

int MoveHistory::getHalfMoveClock() const
{
	return halfMoveClock_;
}

void MoveHistory::clear()
{
	moves_.clear();
	halfMoveClock_ = 0;
}

const std::vector<Move> &MoveHistory::getMoves() const
{
	return moves_;
}
void MoveHistory::setFullMoveNumber(int fullMoveNumber)
{
	fullMoveNumber_ = fullMoveNumber;
}

int MoveHistory::getFullMoveNumber() const
{
	return fullMoveNumber_;
}

void MoveHistory::setHalfMoveClock(int halfMoveClock)
{
	halfMoveClock_ = halfMoveClock;
}

void MoveHistory::halfMoveClockAndReset()
{
	Move lastMove = moves_.back();
	PieceType pieceToMove = lastMove.getPieceType();
	if (pieceToMove == PieceType::PAWN || lastMove.getCapturedPieceType() != PieceType::NONE)
		halfMoveClock_ = 0;
	else
		halfMoveClock_++;
}

void MoveHistory::addMove(const Move &move)
{
	Move recordedMove = move;
	recordedMove.setPreviousHalfMoveClock(halfMoveClock_);
	moves_.push_back(recordedMove);
	halfMoveClockAndReset();
}

std::unique_ptr<Piece> MoveHistory::createCapturedPiece(PieceType type, Color color) const
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

bool MoveHistory::undoMove(Board &board)
{
	if (moves_.empty())
		return false;
	Move move = moves_.back();
	Position tmp = move.getFrom();
	move.setFrom(move.getTo());
	move.setTo(tmp);

	std::unique_ptr<Piece> tmpPiece =
		board[move.getFrom().getX()][move.getFrom().getY()].releasePiece();
	board[move.getTo().getX()][move.getTo().getY()].setPiece(std::move(tmpPiece));
	halfMoveClock_ = move.getPreviousHalfMoveClock();
	if (move.getCapturedPieceType() != PieceType::NONE)
	{
		std::unique_ptr<Piece> capturedPiece = createCapturedPiece(
			move.getCapturedPieceType(),
			board[move.getTo().getX()][move.getTo().getY()].getPiece()->getColor() == Color::White
				? Color::Black
				: Color::White);
		if (capturedPiece == nullptr)
			throw std::runtime_error("Failed to create captured piece.");
		for (size_t moveCount = 0; moveCount < move.getCapturedPieceHasMoved(); ++moveCount)
			capturedPiece->addHasMoved();
		const Position &capturedPosition = move.getCapturedPosition();
		board[capturedPosition.getX()][capturedPosition.getY()].setPiece(std::move(capturedPiece));
	}
	moves_.pop_back();
	Piece *pieceToMove = board[move.getTo().getX()][move.getTo().getY()].getPiece();
	if (pieceToMove != nullptr)
		pieceToMove->subtractHasMoved();

	return true;
}

void MoveHistory::setFEN(const std::string &fen)
{
	if (fen.empty())
		return;
	FEN_ = fen;
}

const std::string &MoveHistory::getFEN() const
{
	return FEN_;
}

void MoveHistory::printMoveHistory() const
{
	if (moves_.empty())
	{
		std::cout << "No moves have been made yet." << std::endl;
		return;
	}

	std::cout << "Move History:" << std::endl;
	for (size_t i = 0; i < moves_.size(); ++i)
	{
		const Move &move = moves_[i];
		std::cout << i + 1 << ". " << move.getFrom().toString() << " -> " << move.getTo().toString() << std::endl;
	}
}

void MoveHistory::addFullMoveNumber()
{
	fullMoveNumber_++;
}

void MoveHistory::updateFEN(const Board &board, Color currentTurn, int halfMoveClock, int fullMoveNumber)
{
	if (moves_.empty())
		return;

	FEN_.clear();

	for (size_t i = 0; i < 8; ++i)
	{
		int emptyCount = 0;

		for (size_t j = 0; j < 8; ++j)
		{
			Piece *piece = board[i][j].getPiece();

			if (piece == nullptr)
			{
				++emptyCount;
			}
			else
			{
				if (emptyCount > 0)
				{
					FEN_ += std::to_string(emptyCount);
					emptyCount = 0;
				}

				char pieceChar = pieceTypeToChar(piece->getType());

				if (piece->getColor() == Color::Black)
					pieceChar = std::tolower(pieceChar);

				FEN_ += pieceChar;
			}
		}

		if (emptyCount > 0)
			FEN_ += std::to_string(emptyCount);

		if (i != 7)
			FEN_ += '/';
	}

	FEN_ += (currentTurn == Color::White) ? " w " : " b ";
	FEN_ += ChessUtils::getCastlingRights(board);
	FEN_ += ' ';

	FEN_ += std::to_string(halfMoveClock);
	FEN_ += ' ';
	FEN_ += std::to_string(fullMoveNumber);

}
