#pragma once

#include "chess/position.hpp"
#include "chess/board.hpp"
#include "chess/utils/chessUtils.hpp"
#include "chess/exceptions/invalidMove.hpp"
#include "chess/exceptions/invalidPositionException.hpp"
#include "chess/exceptions/emptySquareException.hpp"
#include "chess/piece.hpp"
#include "chess/utils/enumPiece.hpp"
#include "chess/pieces/pawn.hpp"
#include "chess/pieces/rook.hpp"
#include "chess/pieces/knight.hpp"
#include "chess/pieces/bishop.hpp"
#include "chess/pieces/queen.hpp"
#include "chess/pieces/king.hpp"

class Move
{
private:
	Position from_;
	Position to_;
	PieceType pieceType_;

	PieceType capturedPieceType_;
	void setPieceType(Piece *piece);

public:
	Move() = default;
	Move(const Position &from, const Position &to);


	void makeMove(Board &board);

	const Position &getFrom() const;
	const Position &getTo() const;
	PieceType getCapturedPieceType() const;
	
	PieceType getPieceType() const;
	std::string toString() const;
	void setFrom(const Position &from);
	void setTo(const Position &to);
};
