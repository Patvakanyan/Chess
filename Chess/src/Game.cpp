#include "chess/Game.hpp"
#include "chess/utils/ChessUtils.hpp"

Game::Game(const std::string fen) : board_(), moveHistory_(), state_(), gameOver_(false)
{
	fen_ = fen;
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
	ChessFenUtils::FENToBoard(fen, board_, state_);
	fen_ = ChessFenUtils::boardToFEN(board_, state_);
}

void Game::undoMove()
{
	if (moveHistory_.getMoves().empty())
		throw InvalidMoveException("No moves to undo.");
	const Move &lastMove = moveHistory_.getMoves().back();
	state_.halfMoveClock = lastMove.getPreviousHalfMoveClock();
	state_.turn = (state_.turn == Color::White) ? Color::Black : Color::White;
	if (state_.turn == Color::Black && state_.fullMoveNumber > 1)
		--state_.fullMoveNumber;
	if (!moveHistory_.undoMove(board_))
	{
		throw InvalidMoveException("No moves to undo.");
	}
	fen_ = ChessFenUtils::boardToFEN(board_, state_);
	gameOver_ = false;
}

void Game::startGame(Color selectedColor)
{
	state_.turn = selectedColor;
	gameOver_ = false;
	moveHistory_.clear();
	board_.initializeBoard();
	stockfishEngine_.startEngine();
	ChessFenUtils::FENToBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", board_, state_);
	state_.turn = selectedColor;
}

int Game::getHalfMoveClock() const
{
	return state_.halfMoveClock;
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
	else if (pieceToMove->getColor() != state_.turn)
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
	state_.turn = (state_.turn == Color::White) ? Color::Black : Color::White;
	moveHistory_.addMove(move);
	state_.enPassantTarget.reset();
	if (move.getPieceType() == PieceType::PAWN &&
		std::abs(move.getFrom().getX() - move.getTo().getX()) == 2)
	{
		state_.enPassantTarget = Position((move.getFrom().getX() + move.getTo().getX()) / 2,
										 move.getFrom().getY());
	}

	if (state_.turn == Color::Black)
		state_.fullMoveNumber++;
	state_.halfMoveClock = (move.getPieceType() == PieceType::PAWN ||
								move.getCapturedPieceType() != PieceType::NONE)
							   ? 0
							   : state_.halfMoveClock + 1;
	fen_ = ChessFenUtils::boardToFEN(board_, state_);
	const Move *lastMove = &moveHistory_.getMoves().back();
	printFEN(std::cout);
	printMoves(std::cout);
	if (ChessGameRules::isCheckmate(board_, state_.turn, lastMove) ||
		ChessGameRules::isStalemate(board_, state_.turn, lastMove) ||
		ChessGameRules::isFiftyMoveRuleDraw(state_.halfMoveClock))
		gameOver_ = true;
}

Color Game::getCurrentTurn() const
{
	return state_.turn;
}

const Move *Game::getLastMove() const
{
	return moveHistory_.getMoves().empty() ? nullptr : &moveHistory_.getMoves().back();
}

StockfishEngine &Game::getStockfishEngine()
{
	return stockfishEngine_;
}

const std::string &Game::getFEN() const
{
	return fen_;
}

void Game::printFEN(std::ostream &out) const
{
	out << fen_ << '\n';
}

void Game::printMoves(std::ostream &out) const
{
	const std::vector<Move> &moves = moveHistory_.getMoves();
	if (moves.empty())
	{
		out << "No moves have been made yet.\n";
		return;
	}

	out << "Move History:\n";
	for (size_t i = 0; i < moves.size(); ++i)
	{
		out << i + 1 << ". " << moves[i].getFrom().toString()
			<< " -> " << moves[i].getTo().toString() << '\n';
	}
}
