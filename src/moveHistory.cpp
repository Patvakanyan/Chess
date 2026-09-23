#include "chess/moveHistory.hpp"

MoveHistory::MoveHistory() : halfMoveClock_(0) {}

int MoveHistory::getHalfMoveClock() const
{
	return halfMoveClock_;
}

const std::vector<Move> &MoveHistory::getMoves() const
{
	return moves_;
}

void MoveHistory::setHalfMoveClock()
{
	Move lastMove = moves_.back();
	PieceType pieceToMove = lastMove.getPieceType();
	if (pieceToMove == PieceType::PAWN)
		halfMoveClock_ = 0;
	else
		halfMoveClock_++;
}

void MoveHistory::addMove(const Move &move)
{

	moves_.push_back(move);
	setHalfMoveClock();
}

bool MoveHistory::undoMove(Board &board)
{
	if (moves_.empty())
		return false;
	Move move = moves_.back();
	Position tmp = move.getFrom();
	move.setFrom(move.getTo());
	move.setTo(tmp);
	std::cout << "Undoing move: " << move.toString() << std::endl;
	Piece *tmpPiece = board[move.getFrom().getX()][move.getFrom().getY()].getPiece();
	board[move.getTo().getX()][move.getTo().getY()].setPiece(tmpPiece);
	if (move.getPieceType() == PieceType::PAWN)
		halfMoveClock_ = 0;
	else
		halfMoveClock_--;
	if (move.getCapturedPieceType() != PieceType::NONE)
	{
		Piece *capturedPiece = nullptr;
		Color capturedPieceColor = (tmpPiece->getColor() == Color::White) ? Color::Black : Color::White;
		switch (move.getCapturedPieceType())
		{
		case PieceType::PAWN:
			capturedPiece = new Pawn(capturedPieceColor);
			break;
		case PieceType::ROOK:
			capturedPiece = new Rook(capturedPieceColor);
			break;
		case PieceType::KNIGHT:
			capturedPiece = new Knight(capturedPieceColor);
			break;
		case PieceType::BISHOP:
			capturedPiece = new Bishop(capturedPieceColor);
			break;
		case PieceType::QUEEN:
			capturedPiece = new Queen(capturedPieceColor);
			break;
		case PieceType::KING:
			capturedPiece = new King(capturedPieceColor);
			break;
		default:
			throw std::invalid_argument("Unknown captured piece type.");
		}
		board[move.getFrom().getX()][move.getFrom().getY()].setPiece(capturedPiece);
	}
	else
	{
		board[move.getFrom().getX()][move.getFrom().getY()].setPiece(nullptr);
	}
	moves_.pop_back();
	Piece *pieceToMove = board[move.getTo().getX()][move.getTo().getY()].getPiece();
	if (pieceToMove != nullptr)
		pieceToMove->subtractHasMoved();

	return true;
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
