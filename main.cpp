#include "include/board.hpp"
#include "include/game.hpp"

int main()
{
	Game game;
	game.startGame(Color::White);
	game.printBoard();
	return 0;
}
