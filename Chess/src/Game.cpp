#include "chess/Game.hpp"
#include "chess/utils/ChessUtils.hpp"

Game::Game(const std::string fen) : board_(), moveHistory_(), currentTurn_(Color::White), gameOver_(false)
{
	moveHistory_.setFEN(fen);
}

Board &Game::getBoard()
{
	return board_;
}
const Board &Game::getBoard() const
{
	return board_;
}

void Game::FENToBoard(const std::string &fen)
{
	ChessUtils::FENToBoard(fen, board_, currentTurn_, moveHistory_);
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
	ChessUtils::FENToBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", board_, currentTurn_, moveHistory_);
	currentTurn_ = selectedColor;
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

bool Game::isPromotionMove(const Position &from, const Position &to) const
{
	const Square *fromSquare = &board_[from.getX()][from.getY()];
	if (fromSquare->isEmpty())
		return false;

	const Piece *movingPiece = fromSquare->getPiece();
	if (movingPiece->getType() != PieceType::PAWN)
		return false;

	int promotionRow = (movingPiece->getColor() == Color::White) ? 0 : 7;
	return to.getX() == promotionRow;
}

void Game::makeMove(const Position &from, const Position &to, PieceType promotionType)
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
		move.makeMove(board_, lastMove, promotionType);
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
	moveHistory_.updateFEN(board_, currentTurn_, moveHistory_.getHalfMoveClock(), moveHistory_.getFullMoveNumber());
	moveHistory_.printMoveHistory();
	std::cout << "Current FEN: " << moveHistory_.getFEN() << std::endl;

	if (currentTurn_ == Color::Black)
		moveHistory_.addFullMoveNumber();
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
