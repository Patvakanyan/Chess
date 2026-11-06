#include "chess/MoveHistory.hpp"

MoveHistory::MoveHistory() = default;

void MoveHistory::clear()
{
	moves_.clear();
}

const std::vector<Move> &MoveHistory::getMoves() const
{
	return moves_;
}
void MoveHistory::addMove(const Move &move)
{
	moves_.push_back(move);
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
	if (move.getCapturedPieceType() != PieceType::NONE)
	{
		std::unique_ptr<Piece> capturedPiece = createCapturedPiece(
			move.getCapturedPieceType(),
			board[move.getTo().getX()][move.getTo().getY()].getPiece()->getColor() == Color::White
				? Color::Black
				: Color::White);
		if (capturedPiece == nullptr)
			throw std::runtime_error("Failed to create captured piece.");
		const Position &capturedPosition = move.getCapturedPosition();
		board[capturedPosition.getX()][capturedPosition.getY()].setPiece(std::move(capturedPiece));
	}
	moves_.pop_back();
	return true;
}

void MoveHistory::printMoveHistory() const
{
}
