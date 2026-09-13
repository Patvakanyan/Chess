#include "include/board.hpp"
#include "include/game.hpp"
#include "include/terminal.hpp"

int main()
{
	Game game;
	Terminal terminal(game);
	terminal.start();
	
	return 0;
}
