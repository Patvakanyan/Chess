#include "chess/Game.hpp"
#include "chess/utils/ChessUtils.hpp"

namespace
{
	void revokeCastlingRight(GameState &state, Color color, int column)
	{
		if (color == Color::White)
		{
			if (column == 0)
				state.castling.whiteQueenside = false;
			else if (column == 7)
				state.castling.whiteKingside = false;
		}
		else
		{
			if (column == 0)
				state.castling.blackQueenside = false;
			else if (column == 7)
				state.castling.blackKingside = false;
		}
	}

	void updateCastlingRights(GameState &state, const Move &move, Color color)
	{
		if (move.getPieceType() == PieceType::KING)
		{
			if (color == Color::White)
			{
				state.castling.whiteKingside = false;
				state.castling.whiteQueenside = false;
			}
			else
			{
				state.castling.blackKingside = false;
				state.castling.blackQueenside = false;
			}
		}
		else if (move.getPieceType() == PieceType::ROOK &&
				 (move.getFrom().getY() == 0 || move.getFrom().getY() == 7))
		{
			revokeCastlingRight(state, color, move.getFrom().getY());
		}

		if (move.getCapturedPieceType() == PieceType::ROOK &&
			(move.getCapturedPosition().getY() == 0 || move.getCapturedPosition().getY() == 7))
		{
			const Color capturedColor = oppositeColor(color);
			revokeCastlingRight(state, capturedColor, move.getCapturedPosition().getY());
		}
	}
}

Game::Game(const std::string fen)
	: board_(), moveHistory_(), state_(), gameOver_(false),
	  result_(ChessGameResult::Ongoing)
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
	moveHistory_.clear();
	gameOver_ = false;
	result_ = ChessGameResult::Ongoing;
	fen_ = ChessFenUtils::boardToFEN(board_, state_);
}

void Game::undoMove()
{
	if (moveHistory_.empty())
		throw InvalidMoveException("No moves to undo.");
	const Move &lastMove = moveHistory_.back();
	state_.halfMoveClock = lastMove.getPreviousHalfMoveClock();
	state_.turn = oppositeColor(state_.turn);
	if (state_.turn == Color::Black && state_.fullMoveNumber > 1)
		--state_.fullMoveNumber;
	lastMove.undoMove(board_);
	moveHistory_.pop_back();
	fen_ = ChessFenUtils::boardToFEN(board_, state_);
	gameOver_ = false;
	result_ = ChessGameResult::Ongoing;
}

void Game::startGame()
{
	gameOver_ = false;
	result_ = ChessGameResult::Ongoing;
	moveHistory_.clear();
	board_.initializeBoard();
	state_ = GameState();
	ChessFenUtils::FENToBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", board_, state_);
	fen_ = ChessFenUtils::boardToFEN(board_, state_);
	stockfishEngine_.startEngine();
}

int Game::getHalfMoveClock() const
{
	return state_.halfMoveClock;
}

bool Game::isPromotionMove(const Position &from, const Position &to) const
{
	return ChessMoveUtils::isPromotionMove(board_, from, to);
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
	if (pieceToMove->getType() == PieceType::KING && from.getY() == 4 &&
		(to.getY() == 2 || to.getY() == 6))
	{
		const bool kingside = to.getY() == 6;
		const bool allowed = canCastle(pieceToMove->getColor(), kingside);
		if (!allowed)
			throw InvalidMoveException("Castling rights have been lost.");
	}
	Move move(from, to);
	try
	{
		const Move *lastMove = moveHistory_.empty()
								   ? nullptr
								   : &moveHistory_.back();
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
	state_.turn = oppositeColor(state_.turn);
	updateCastlingRights(state_, move, pieceToMove->getColor());
	moveHistory_.push_back(move);
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
	const Move *lastMove = &moveHistory_.back();
	printFEN(std::cout);
	printMoves(std::cout);
	if (ChessGameRules::isCheckmate(board_, state_.turn, lastMove))
		result_ = ChessGameResult::Checkmate;
	else if (ChessGameRules::isStalemate(board_, state_.turn, lastMove))
		result_ = ChessGameResult::Stalemate;
	else if (ChessGameRules::isFiftyMoveRuleDraw(state_.halfMoveClock))
		result_ = ChessGameResult::Draw;
	else
		result_ = ChessGameResult::Ongoing;
	gameOver_ = result_ != ChessGameResult::Ongoing;
}

std::vector<Position> Game::legalMovesFrom(const Position &from)
{
	const Piece *piece = board_[from.getX()][from.getY()].getPiece();
	if (piece == nullptr || piece->getColor() != state_.turn)
		return {};

	return ChessGameRules::legalMovesFrom(
		board_, from, getLastMove(),
		canCastle(piece->getColor(), false),
		canCastle(piece->getColor(), true));
}

bool Game::isGameOver() const
{
	return gameOver_;
}

ChessGameResult Game::getResult() const
{
	return result_;
}

Color Game::getCurrentTurn() const
{
	return state_.turn;
}

const Move *Game::getLastMove() const
{
	return moveHistory_.empty() ? nullptr : &moveHistory_.back();
}

bool Game::canCastle(Color color, bool kingside) const
{
	if (color == Color::White)
		return kingside ? state_.castling.whiteKingside : state_.castling.whiteQueenside;
	return kingside ? state_.castling.blackKingside : state_.castling.blackQueenside;
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
	const std::vector<Move> &moves = moveHistory_;
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
