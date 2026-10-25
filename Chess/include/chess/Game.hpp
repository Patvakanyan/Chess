#pragma once

#include "chess/Board.hpp"
#include "chess/utils/Color.hpp"
#include "chess/Position.hpp"
#include "chess/MoveHistory.hpp"
#include "chess/Move.hpp"

#include "chess/exceptions/EmptySquareException.hpp"
#include "chess/exceptions/InvalidPositionException.hpp"


#include <iostream>

class Game
{
private:
	Board board_;
	MoveHistory moveHistory_;

	Color currentTurn_;
	bool gameOver_;

	void prepareBoard();

public:
	Game();
	Game(const Game &other) = delete;
	Game &operator=(const Game &other) = delete;
	Game(Game &&other) = delete;
	Game &operator=(Game &&other) = delete;
	~Game() = default;

	Board &getBoard();
	const Board &getBoard() const;

	int getHalfMoveClock() const;
	void startGame(Color selectedColor);
	void makeMove(const Position &from, const Position &to);
	void undoMove();
	Color getCurrentTurn() const;
	const Move *getLastMove() const;
	void printBoard() const;
};
