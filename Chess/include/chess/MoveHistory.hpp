#pragma once

#include "chess/Move.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/Piece.hpp"
#include "chess/utils/ChessUtils.hpp"

#include "chess/exceptions/InvalidMoveException.hpp"
#include "chess/utils/EnumPiece.hpp"
#include <vector>
#include <memory>

#include <iostream>

class Move;

class MoveHistory
{
private:
	std::vector<Move> moves_;
	int halfMoveClock_;
	int fullMoveNumber_;

	std::string FEN_;

	void halfMoveClockAndReset();
	std::unique_ptr<Piece> createCapturedPiece(PieceType type, Color color) const;

public:
	MoveHistory();
	MoveHistory(const MoveHistory &other) = delete;
	MoveHistory &operator=(const MoveHistory &other) = delete;
	MoveHistory(MoveHistory &&other) = delete;
	MoveHistory &operator=(MoveHistory &&other) = delete;
	~MoveHistory() = default;


	void setFEN(const std::string &fen);
	const std::string &getFEN() const;

	void updateFEN(const Board &board, Color currentTurn, int halfMoveClock, int fullMoveNumber);//Add this method to update the FEN string based on the current board state and move history

	void setFullMoveNumber(int fullMoveNumber);
	int getFullMoveNumber() const;
	void setHalfMoveClock(int halfMoveClock);
	int getHalfMoveClock() const;

	void addFullMoveNumber();

	void clear();
	void addMove(const Move &move);
	bool undoMove(Board &board);
	const std::vector<Move> &getMoves() const;
	void printMoveHistory() const;
};
