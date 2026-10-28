#pragma once

#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/Square.hpp"
#include "chess/exceptions/InvalidMoveException.hpp"
#include "chess/exceptions/EmptySquareException.hpp"
#include "chess/Piece.hpp"
#include "chess/utils/EnumPiece.hpp"
#include "chess/utils/ChessUtils.hpp"

class Move
{
private:
	Position from_;
	Position to_;
	PieceType pieceType_;

	PieceType capturedPieceType_;
	size_t capturedPieceHasMoved_;
	Position capturedPosition_;
	CustomMoveType capturedPieceMoveType_;

	int previousHalfMoveClock_;

private:
	void setPieceType(Piece *piece);
	void validateMove(Board &board, Square *toSquare, Piece *pieceToMove,
					  CustomMoveType type) const;
	void applyMove(Board &board, Square *fromSquare, Square *toSquare, Piece *pieceToMove, CustomMoveType type);
	bool isEnPassant(const Board &board, const Move *lastMove) const;
	bool isCastleKingside(const Board &board) const;
	bool isCastleQueenside(const Board &board) const;

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
