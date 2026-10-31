#pragma once

#include "chess/Board.hpp"
#include "chess/utils/Color.hpp"
#include "chess/Position.hpp"
#include "chess/MoveHistory.hpp"
#include "chess/Move.hpp"
#include "chess/exceptions/InvalidMoveException.hpp"
#include "chess/exceptions/InvalidPositionException.hpp"
#include <string>

class Game
{
private:
	Board board_;
	MoveHistory moveHistory_;

	Color currentTurn_;
	bool gameOver_;

	void prepareBoard();

public:
	Game(std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Game(const Game &other) = delete;
	Game &operator=(const Game &other) = delete;
	Game(Game &&other) = delete;
	Game &operator=(Game &&other) = delete;
	~Game() = default;

	Board &getBoard();
	const Board &getBoard() const;

	void FENToBoard(const std::string &fen);
	int getHalfMoveClock() const;
	void startGame(Color selectedColor);
	bool isPromotionMove(const Position &from, const Position &to) const;
	void makeMove(const Position &from, const Position &to, PieceType promotionType = PieceType::NONE);
	void undoMove();
	Color getCurrentTurn() const;
	const Move *getLastMove() const;
};
