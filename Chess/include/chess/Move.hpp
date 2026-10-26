#pragma once

#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/Square.hpp"
#include "chess/exceptions/InvalidMoveException.hpp"
#include "chess/exceptions/EmptySquareException.hpp"
#include "chess/Piece.hpp"
#include "chess/utils/EnumPiece.hpp"

class Move
{
private:
	Position from_;
	Position to_;
	PieceType pieceType_;

	PieceType capturedPieceType_;
	size_t capturedPieceHasMoved_;
	Position capturedPosition_;
	
	int previousHalfMoveClock_;
	void setPieceType(Piece *piece);
	void validateMove(Board &board, Square *toSquare, Piece *pieceToMove,
					  bool enPassant) const;
	void applyMove(Board &board, Square *fromSquare, Square *toSquare, Piece *pieceToMove, bool enPassant);
	bool isEnPassant(const Board &board, const Move *lastMove) const;

public:
	Move();
	Move(const Position &from, const Position &to);

	void makeMove(Board &board, const Move *lastMove = nullptr);

	const Position &getFrom() const;
	const Position &getTo() const;
	PieceType getCapturedPieceType() const;
	size_t getCapturedPieceHasMoved() const;
	const Position &getCapturedPosition() const;
	int getPreviousHalfMoveClock() const;
	void setPreviousHalfMoveClock(int halfMoveClock);

	PieceType getPieceType() const;
	std::string toString() const;
	void setFrom(const Position &from);
	void setTo(const Position &to);
};
