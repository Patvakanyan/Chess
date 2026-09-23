#include "chess/game.hpp"

Game::Game() : moveHistory_(), currentTurn_(Color::White), gameOver_(false)
{
}

Board &Game::getBoard()
{
	return board_;
}

const Board &Game::getBoard() const
{
	return board_;
}

void Game::prepareBoard()
{
	board_.placePiece(new King(Color::White), Position(7, 4));
	board_.placePiece(new King(Color::Black), Position(0, 4));
	board_.placePiece(new Rook(Color::White), Position(7, 0));
	board_.placePiece(new Rook(Color::White), Position(7, 7));
	board_.placePiece(new Rook(Color::Black), Position(0, 0));
	board_.placePiece(new Rook(Color::Black), Position(0, 7));
	board_.placePiece(new Knight(Color::White), Position(7, 1));
	board_.placePiece(new Knight(Color::White), Position(7, 6));
	board_.placePiece(new Knight(Color::Black), Position(0, 1));
	board_.placePiece(new Knight(Color::Black), Position(0, 6));
	board_.placePiece(new Bishop(Color::White), Position(7, 2));
	board_.placePiece(new Bishop(Color::White), Position(7, 5));
	board_.placePiece(new Bishop(Color::Black), Position(0, 2));
	board_.placePiece(new Bishop(Color::Black), Position(0, 5));
	board_.placePiece(new Queen(Color::White), Position(7, 3));
	board_.placePiece(new Queen(Color::Black), Position(0, 3));
	for (int i = 0; i < 8; ++i)
	{
		board_.placePiece(new Pawn(Color::White), Position(6, i));
		board_.placePiece(new Pawn(Color::Black), Position(1, i));
	}
}

void Game::undoMove()
{
	currentTurn_ = (currentTurn_ == Color::White) ? Color::Black : Color::White;
	if (!moveHistory_.undoMove(board_))
	{
		currentTurn_ = (currentTurn_ == Color::White) ? Color::Black : Color::White;
		throw InvalidMoveException("No moves to undo.");
	}
}

void Game::startGame(Color selectedColor)
{
	currentTurn_ = selectedColor;
	gameOver_ = false;
	board_.initializeBoard();

	prepareBoard();
}

int Game::getHalfMoveClock() const
{
	return moveHistory_.getHalfMoveClock();
}

void Game::printBoard() const
{
	std::cout << "  a b c d e f g h\n";

	for (int i = 0; i < 8; ++i)
	{
		std::cout << 8 - i << " ";
		for (int j = 0; j < 8; ++j)
		{
			const Square &square = board_[i][j];
			if (square.isEmpty())
			{
				std::cout << ". ";
			}
			else
			{
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
					pieceChar = std::tolower(pieceChar);
				}

				std::cout << pieceChar << " ";
			}
		}
		std::cout << 8 - i << "\n";
	}
	std::cout << "  a b c d e f g h\n";
}

void Game::makeMove(const Position &from, const Position &to)
{
	if (gameOver_)
	{
		throw std::runtime_error("Game is over. No more moves can be made.");
	}
	Move move(from, to);
	try
	{
		move.makeMove(board_);
	}
	catch (const InvalidMoveException &e)
	{
		throw InvalidMoveException("Invalid move: " + std::string(e.what()));
	}
	catch (const InvalidPositionException &e)
	{
		throw InvalidPositionException("Invalid position: " + std::string(e.what()));
	}
	catch (const EmptySquareException &e)
	{
		throw EmptySquareException("Empty square: " + std::string(e.what()));
	}
	currentTurn_ = (currentTurn_ == Color::White) ? Color::Black : Color::White;
	moveHistory_.addMove(move);
	moveHistory_.printMoveHistory();
}


Color Game::getCurrentTurn() const
{
	return currentTurn_;
}
