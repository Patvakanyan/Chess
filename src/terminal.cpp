#include "../include/terminal.hpp"

Terminal::Terminal(Game &game) : game_(game) {}

void Terminal::printWelcomeMessage() const
{
	std::cout << "Welcome to Chess!" << std::endl;
	std::cout << "Select your color (White/Black): ";

	std::string colorInput;
	if (std::getline(std::cin, colorInput) && !colorInput.empty())
	{
		Color selectedColor = (colorInput == "White" || colorInput == "white") ? Color::White : Color::Black;
		game_.startGame(selectedColor);
	}
	else
	{
		std::cout << "Invalid input. Defaulting to White." << std::endl;
		game_.startGame(Color::White);
	}
}

void Terminal::printCurrentTurn() const
{
	Color currentTurn = game_.getCurrentTurn();
	std::string turnColor = (currentTurn == Color::White) ? "White" : "Black";
	std::cout << "Current Turn: " << turnColor << std::endl;
}

void Terminal::printCheckmateMessage(Color winner) const
{
	std::string winnerColor = (winner == Color::White) ? "White" : "Black";
	std::cout << "Checkmate! " << winnerColor << " wins!" << std::endl;
}

void Terminal::printStalemateMessage() const
{
	std::cout << "Stalemate! The game is a draw." << std::endl;
}

void Terminal::printInvalidMoveMessage() const
{
	std::cout << "Invalid move. Please try again." << std::endl;
}
void Terminal::printGameOverMessage() const
{
	std::cout << "Game Over!" << std::endl;
}

void Terminal::printGameResult() const
{
	if (game_.isCheckmate(Color::White))
	{
		printCheckmateMessage(Color::Black);
	}
	else if (game_.isCheckmate(Color::Black))
	{
		printCheckmateMessage(Color::White);
	}
	else if (game_.isStalemate(Color::White) || game_.isStalemate(Color::Black))
	{
		printStalemateMessage();
	}
	else
	{
		std::cout << "Game is still ongoing." << std::endl;
	}
}

void Terminal::clearScreen()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}
void Terminal::start()
{
	printWelcomeMessage();

	while (true)
	{
		printCurrentTurn();
		game_.printBoard();

		std::string input;

		std::cout << "Enter your move (e.g., e2 e4): ";

		if (!std::getline(std::cin, input))
		{
			std::cout << "\nExiting game." << std::endl;
			break;
		}
		std::istringstream iss(input);
		std::string fromInput;
		std::string toInput;

		if (!(iss >> fromInput >> toInput))
		{
			std::cout << "Invalid input. Please try again." << std::endl;
			continue;
		}

		try
		{
			Position from(fromInput);
			Position to(toInput);
			game_.makeMove(from, to);
		}
		catch (const std::invalid_argument &e)
		{
			std::cout << "Error: " << e.what() << std::endl;
			continue;
		}

		if (game_.isCheckmate(Color::White) ||
			game_.isCheckmate(Color::Black) ||
			game_.isStalemate(Color::White) ||
			game_.isStalemate(Color::Black))
		{
			printGameResult();
			break;
		}
	}
}
