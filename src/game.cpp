#include "../include/game.hpp"

Game::Game() : currentTurn_(Color::White), gameOver_(false)
{
}

void Game::prepareBoard()
{
	board_.placePiece(new King(Color::White), Position(0, 4));
	board_.placePiece(new King(Color::Black), Position(7, 4));
	board_.placePiece(new Rook(Color::White), Position(0, 0));
	board_.placePiece(new Rook(Color::White), Position(0, 7));
	board_.placePiece(new Rook(Color::Black), Position(7, 0));
	board_.placePiece(new Rook(Color::Black), Position(7, 7));
	board_.placePiece(new Knight(Color::White), Position(0, 1));
	board_.placePiece(new Knight(Color::White), Position(0, 6));
	board_.placePiece(new Knight(Color::Black), Position(7, 1));
	board_.placePiece(new Knight(Color::Black), Position(7, 6));
	board_.placePiece(new Bishop(Color::White), Position(0, 2));
	board_.placePiece(new Bishop(Color::White), Position(0, 5));
	board_.placePiece(new Bishop(Color::Black), Position(7, 2));
	board_.placePiece(new Bishop(Color::Black), Position(7, 5));
	board_.placePiece(new Queen(Color::White), Position(0, 3));
	board_.placePiece(new Queen(Color::Black), Position(7, 3));
	for (int i = 0; i < 8; ++i)
	{
		board_.placePiece(new Pawn(Color::White), Position(1, i));
		board_.placePiece(new Pawn(Color::Black), Position(6, i));
	}
}

void Game::startGame(Color selectedColor)
{
	currentTurn_ = Color::White;
	selectedColor_ = selectedColor;
	gameOver_ = false;
	board_.initializeBoard();

	prepareBoard();
}

void Game::printBoard() const
{
	std::cout << "  a b c d e f g h\n";

	for (int row = 7; row >= 0; --row)
	{
		std::cout << row + 1 << " ";

		for (int column = 0; column < 8; ++column)
		{
			const Square &square = board_[row][column];
			if (square.isEmpty())
			{
				std::cout << ". ";
				continue;
			}

			const Piece *piece = square.getPiece();
			char pieceChar = '?';
			if (dynamic_cast<const King *>(piece))
			{
				pieceChar = 'K';
			}
			else if (dynamic_cast<const Queen *>(piece))
			{
				pieceChar = 'Q';
			}
			else if (dynamic_cast<const Rook *>(piece))
			{
				pieceChar = 'R';
			}
			else if (dynamic_cast<const Bishop *>(piece))
			{
				pieceChar = 'B';
			}
			else if (dynamic_cast<const Knight *>(piece))
			{
				pieceChar = 'N';
			}
			else if (dynamic_cast<const Pawn *>(piece))
			{
				pieceChar = 'P';
			}

			if (piece->getColor() == Color::Black)
			{
				pieceChar = static_cast<char>(pieceChar - 'A' + 'a');
			}
			std::cout << pieceChar << " ";
		}

		std::cout << row + 1 << '\n';
	}

	std::cout << "  a b c d e f g h\n";
}

void Game::makeMove(const Position &from, const Position &to)
{
	if (gameOver_)
	{
		throw std::runtime_error("Game is over. No more moves can be made.");
	}
	board_.movePiece(from, to);
	currentTurn_ = (currentTurn_ == Color::White) ? Color::Black : Color::White;
}

bool Game::isCheckmate(Color) const
{
	return false;
}

bool Game::isStalemate(Color) const
{
	return false;
}

Color Game::getCurrentTurn() const
{
	return currentTurn_;
}
