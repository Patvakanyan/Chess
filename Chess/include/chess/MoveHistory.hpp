#pragma once

#include "chess/Move.hpp"
#include "chess/Position.hpp"
#include "chess/Board.hpp"
#include "chess/Piece.hpp"
#include "chess/utils/ChessUtils.hpp"
#include "chess/GameState.hpp"

#include "chess/exceptions/InvalidMoveException.hpp"
#include "chess/utils/EnumPiece.hpp"
#include <vector>
#include <memory>


class Move;

class MoveHistory
{
private:
	std::vector<Move> moves_;

	std::unique_ptr<Piece> createCapturedPiece(PieceType type, Color color) const;

public:
	MoveHistory();
	MoveHistory(const MoveHistory &other) = delete;
	MoveHistory &operator=(const MoveHistory &other) = delete;
	MoveHistory(MoveHistory &&other) = delete;
	MoveHistory &operator=(MoveHistory &&other) = delete;
	~MoveHistory() = default;

	void clear();
	void addMove(const Move &move);
	bool undoMove(Board &board);

	const std::vector<Move> &getMoves() const;
	void printMoveHistory() const;
};
