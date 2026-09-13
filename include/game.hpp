#pragma once

#include "./board.hpp"
#include "./color.hpp"
#include "./position.hpp"
#include "./piece.hpp"
#include "./king.hpp"
#include "./knight.hpp"
#include "./rook.hpp"
#include "./bishop.hpp"
#include "./queen.hpp"
#include "./pawn.hpp"
#include <iostream>
#include <vector>



class Game
{
private:
	Board board_;
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

	void startGame(Color selectedColor);
	void makeMove(const Position &from, const Position &to);
	bool isCheckmate(Color color) const;
	bool isStalemate(Color color) const;
	Color getCurrentTurn() const;
	void printBoard() const;
};
