#pragma once

#include "chess/Game.hpp"
#include "chess/utils/ChessGameResult.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

class Terminal
{
private:
	Game &game_;

public:
	Terminal(Game &game);

	PieceType askPromotionPiece() const;
	void printWelcomeMessage() const;
	void printCurrentTurn() const;
	void printCheckmateMessage(Color winner) const;
	void printStalemateMessage() const;
	void printInvalidMoveMessage() const;
	void printGameOverMessage() const;
	void printGameResult(ChessGameResult result) const;
	void start();
	void clearScreen();
};
