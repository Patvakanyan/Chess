#pragma once
#include "chess/Board.hpp"
#include "chess/utils/Color.hpp"
#include "chess/Position.hpp"
#include "chess/Piece.hpp"
#include "pieces/King.hpp"
#include "pieces/Knight.hpp"
#include "pieces/Rook.hpp"
#include "pieces/Bishop.hpp"
#include "pieces/Queen.hpp"
#include "pieces/Pawn.hpp"
#include "MoveHistory.hpp"
#include "chess/exceptions/InvalidMove.hpp"
#include "chess/exceptions/InvalidPositionException.hpp"
#include "chess/exceptions/EmptySquareException.hpp"
#include <iostream>
#include <vector>

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
	void printBoard() const;
};
