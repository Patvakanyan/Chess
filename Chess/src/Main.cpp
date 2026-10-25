#include "chess/Board.hpp"
#include "chess/Game.hpp"
#include "chess/Terminal.hpp"

int main()
{
	Game game;
	Terminal terminal(game);
	terminal.start();
	
	return 0;
}
