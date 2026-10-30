#include "chess/Terminal.hpp"
#include "chess/exceptions/EmptySquareException.hpp"
#include "chess/exceptions/InvalidPositionException.hpp"
#include "chess/utils/ChessUtils.hpp"

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

PieceType Terminal::askPromotionPiece() const
{
	std::cout << "Pawn promotion! Choose a piece (Q, R, B, N): ";
	std::string input;
	while (true)
	{
		if (!std::getline(std::cin, input) || input.empty())
		{
			std::cout << "Invalid input. Please try again: ";
			continue;
		}

		char choice = std::toupper(input[0]);
		switch (choice)
		{
		case 'Q':
			return PieceType::QUEEN;
		case 'R':
			return PieceType::ROOK;
		case 'B':
			return PieceType::BISHOP;
		case 'N':
			return PieceType::KNIGHT;
		default:
			std::cout << "Invalid choice. Please choose Q, R, B, or N: ";
			break;
		}
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

void Terminal::printGameResult(ChessGameResult result) const
{
	switch (result)
	{
	case ChessGameResult::Checkmate:
		printCheckmateMessage(game_.getCurrentTurn() == Color::White ? Color::Black : Color::White);
		break;
	case ChessGameResult::Stalemate:
		printStalemateMessage();
		break;
	case ChessGameResult::Draw:
		std::cout << "The game is a draw." << std::endl;
		break;
	case ChessGameResult::Resignation:
		std::cout << "A player has resigned. Game over." << std::endl;
		break;
	case ChessGameResult::Timeout:
		std::cout << "A player has run out of time. Game over." << std::endl;
		break;
	default:
		std::cout << "The game is ongoing." << std::endl;
		break;
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
		std::string extraInput;

		if (input == "undo")
		{
			try
			{
				game_.undoMove();
				continue;
			}
			catch (const std::exception &e)
			{
				std::cerr << "Error: " << e.what() << std::endl;
				continue;
			}
		}
		if (!(iss >> fromInput >> toInput) || (iss >> extraInput))
		{
			std::cout << "Invalid input. Please try again." << std::endl;
			continue;
		}

		try
		{
			Position from(fromInput);
			Position to(toInput);
			if (game_.isPromotionMove(from, to))
			{
				PieceType promotionType = askPromotionPiece();
				game_.makeMove(from, to, promotionType);
			}
			else
				game_.makeMove(from, to);
		}
		catch (const InvalidMoveException &e)
		{
			std::cerr << "Invalid move: " << e.what() << std::endl;
			printInvalidMoveMessage();
			continue;
		}
		catch (const InvalidPositionException &e)
		{
			std::cerr << "Invalid position: " << e.what() << std::endl;
			printInvalidMoveMessage();
			continue;
		}
		catch (const EmptySquareException &e)
		{
			std::cerr << "Empty square: " << e.what() << std::endl;
			printInvalidMoveMessage();
			continue;
		}
		catch (const std::exception &e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
			printInvalidMoveMessage();
			continue;
		}

		Color sideToMove = game_.getCurrentTurn();
		const Move *lastMove = game_.getLastMove();
		if (ChessUtils::isCheckmate(game_.getBoard(), sideToMove, lastMove))
		{
			printGameResult(ChessGameResult::Checkmate);
			break;
		}

		if (ChessUtils::isFiftyMoveRuleDraw(game_.getHalfMoveClock()))
		{
			printGameResult(ChessGameResult::Draw);
			break;
		}

		if (ChessUtils::isStalemate(game_.getBoard(), sideToMove, lastMove))
		{
			printGameResult(ChessGameResult::Stalemate);
			break;
		}
	}
}
