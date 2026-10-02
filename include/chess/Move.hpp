#pragma once

#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/utils/ChessUtils.hpp"
#include "chess/exceptions/InvalidMove.hpp"
#include "chess/exceptions/InvalidPositionException.hpp"
#include "chess/exceptions/EmptySquareException.hpp"
#include "chess/Piece.hpp"
#include "chess/utils/EnumPiece.hpp"
#include "chess/pieces/Pawn.hpp"
#include "chess/pieces/Rook.hpp"
#include "chess/pieces/Knight.hpp"
#include "chess/pieces/Bishop.hpp"
#include "chess/pieces/Queen.hpp"
#include "chess/pieces/King.hpp"

class Move
{
private:
	Position from_;
	Position to_;
	PieceType pieceType_;

	PieceType capturedPieceType_;
	size_t capturedPieceHasMoved_;
	void setPieceType(Piece *piece);

public:
	Move() = default;
	Move(const Position &from, const Position &to);


	void makeMove(Board &board);

	const Position &getFrom() const;
	const Position &getTo() const;
	PieceType getCapturedPieceType() const;
	size_t getCapturedPieceHasMoved() const;
	
	PieceType getPieceType() const;
	std::string toString() const;
	void setFrom(const Position &from);
	void setTo(const Position &to);
};
