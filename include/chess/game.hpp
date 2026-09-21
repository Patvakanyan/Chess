#pragma once

#include "chess/board.hpp"
#include "chess/color.hpp"
#include "chess/position.hpp"
#include "chess/piece.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"
#include "pieces/rook.hpp"
#include "pieces/bishop.hpp"
#include "pieces/queen.hpp"
#include "pieces/pawn.hpp"
#include "moveHistory.hpp"
#include "chess/exceptions/invalidMove.hpp"
#include "chess/exceptions/invalidPositionException.hpp"
#include "chess/exceptions/emptySquareException.hpp"
#include <iostream>
#include <vector>

class Game
{
private:
	Board board_;
	MoveHistory moveHistory_;

	Color currentTurn_;
	Color selectedColor_;
	bool gameOver_;

	void prepareBoard();

public:
	Game();
	Game(const Game &other) = default;
	Game &operator=(const Game &other) = default;
	Game(Game &&other) = default;
	Game &operator=(Game &&other) = default;
	~Game() = default;

	void startGame(Color selectedColor);
	void makeMove(const Position &from, const Position &to);
	void undoMove();
	Color getCurrentTurn() const;
	void printBoard() const;
};
