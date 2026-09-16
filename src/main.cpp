#include "chess/board.hpp"
#include "chess/game.hpp"
#include "chess/terminal.hpp"

int main()
{
	Game game;
	Terminal terminal(game);
	terminal.start();
	
	return 0;
}
