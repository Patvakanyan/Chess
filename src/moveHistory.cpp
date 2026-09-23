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
	board[move.getFrom().getX()][move.getFrom().getY()].setPiece(nullptr);
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
