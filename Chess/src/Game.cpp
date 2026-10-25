#include "chess/Game.hpp"
#include "chess/utils/ChessUtils.hpp"


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
	board_.placePiece(std::make_unique<King>(Color::White), Position(7, 4));
	board_.placePiece(std::make_unique<King>(Color::Black), Position(0, 4));
	board_.placePiece(std::make_unique<Rook>(Color::White), Position(7, 0));
	board_.placePiece(std::make_unique<Rook>(Color::White), Position(7, 7));
	board_.placePiece(std::make_unique<Rook>(Color::Black), Position(0, 0));
	board_.placePiece(std::make_unique<Rook>(Color::Black), Position(0, 7));
	board_.placePiece(std::make_unique<Knight>(Color::White), Position(7, 1));
	board_.placePiece(std::make_unique<Knight>(Color::White), Position(7, 6));
	board_.placePiece(std::make_unique<Knight>(Color::Black), Position(0, 1));
	board_.placePiece(std::make_unique<Knight>(Color::Black), Position(0, 6));
	board_.placePiece(std::make_unique<Bishop>(Color::White), Position(7, 2));
	board_.placePiece(std::make_unique<Bishop>(Color::White), Position(7, 5));
	board_.placePiece(std::make_unique<Bishop>(Color::Black), Position(0, 2));
	board_.placePiece(std::make_unique<Bishop>(Color::Black), Position(0, 5));
	board_.placePiece(std::make_unique<Queen>(Color::White), Position(7, 3));
	board_.placePiece(std::make_unique<Queen>(Color::Black), Position(0, 3));
	for (int i = 0; i < 8; ++i)
	{
		board_.placePiece(std::make_unique<Pawn>(Color::White), Position(6, i));
		board_.placePiece(std::make_unique<Pawn>(Color::Black), Position(1, i));
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
	gameOver_ = false;
}

void Game::startGame(Color selectedColor)
{
	currentTurn_ = selectedColor;
	gameOver_ = false;
	moveHistory_.clear();
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
					pieceChar = 'K';
				else if (dynamic_cast<const Queen *>(piece))
					pieceChar = 'Q';
				else if (dynamic_cast<const Rook *>(piece))
					pieceChar = 'R';
				else if (dynamic_cast<const Bishop *>(piece))
					pieceChar = 'B';
				else if (dynamic_cast<const Knight *>(piece))
					pieceChar = 'N';
				else if (dynamic_cast<const Pawn *>(piece))
					pieceChar = 'P';

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
		throw std::runtime_error("Game is over. No more moves can be made.");
	if (!board_.isValidPosition(from) || !board_.isValidPosition(to))
		throw InvalidPositionException("Invalid position(s) for the move.");

	Piece *pieceToMove = board_[from.getX()][from.getY()].getPiece();
	if (pieceToMove == nullptr)
		throw EmptySquareException("No piece at the source position.");
	else if (pieceToMove->getColor() != currentTurn_)
		throw InvalidMoveException("It's not your turn to move.");
	Move move(from, to);
	try
	{
		const Move *lastMove = moveHistory_.getMoves().empty()
			? nullptr
			: &moveHistory_.getMoves().back();
		move.makeMove(board_,  lastMove);
	}
	catch (const InvalidMoveException &e)
	{
		throw InvalidMoveException("Invalid move: " + std::string(e.what()));
	}
	catch (const InvalidPositionException &e)
	{
		throw InvalidPositionException("Invalid position: " + std::string(e.what()));
	}
	currentTurn_ = (currentTurn_ == Color::White) ? Color::Black : Color::White;
	moveHistory_.addMove(move);
	moveHistory_.printMoveHistory();

	const Move *lastMove = &moveHistory_.getMoves().back();
	if (ChessUtils::isCheckmate(board_, currentTurn_, lastMove) ||
		ChessUtils::isStalemate(board_, currentTurn_, lastMove) ||
		ChessUtils::isFiftyMoveRuleDraw(moveHistory_.getHalfMoveClock()))
		gameOver_ = true;
}


Color Game::getCurrentTurn() const
{
	return currentTurn_;
}

const Move *Game::getLastMove() const
{
	return moveHistory_.getMoves().empty() ? nullptr : &moveHistory_.getMoves().back();
}
