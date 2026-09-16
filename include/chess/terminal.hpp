#pragma once

#include "chess/game.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
class Terminal
{
private:
	Game &game_;
public:
	Terminal(Game& game);

	void printWelcomeMessage() const;
	void printCurrentTurn() const;
	void printCheckmateMessage(Color winner) const;
	void printStalemateMessage() const;
	void printInvalidMoveMessage() const;
	void printGameOverMessage() const;
	void printGameResult() const;
	void start();
	void clearScreen();

};
