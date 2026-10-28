#include "GameController.hpp"

#include "chess/Board.hpp"
#include "chess/Piece.hpp"
#include "chess/Square.hpp"
#include "chess/exceptions/InvalidMoveException.hpp"
#include "chess/utils/ChessUtils.hpp"

#include <memory>

namespace
{
struct CandidateMove
{
	Position destination;
	bool enPassant = false;
	bool castling = false;
};

// Engine rows/columns already match the GUI's: row 0 is rank 8 (top, black
// side), column 0 is the a-file (left), same as ChessScene. No flipping.
PieceKind toKind(PieceType type)
{
	switch (type)
	{
	case PieceType::KING:
		return PieceKind::King;
	case PieceType::QUEEN:
		return PieceKind::Queen;
	case PieceType::ROOK:
		return PieceKind::Rook;
	case PieceType::BISHOP:
		return PieceKind::Bishop;
	case PieceType::KNIGHT:
		return PieceKind::Knight;
	case PieceType::PAWN:
	default:
		return PieceKind::Pawn;
	}
}

class TemporaryMove
{
public:
	TemporaryMove(Board &board, const Position &from, const Position &to,
				  bool enPassant, bool castling)
		: fromSquare(board[from.getX()][from.getY()]),
		  toSquare(board[to.getX()][to.getY()]),
		  captureSquare(enPassant ? board[from.getX()][to.getY()] : toSquare),
		  movingPiece(fromSquare.releasePiece()),
		  capturedPiece(captureSquare.releasePiece())
	{
		toSquare.setPiece(std::move(movingPiece));
		toSquare.getPiece()->addHasMoved();

		if (castling)
		{
			rookFromSquare = &board[from.getX()][to.getY() == 6 ? 7 : 0];
			rookToSquare = &board[from.getX()][to.getY() == 6 ? 5 : 3];
			rookPiece = rookFromSquare->releasePiece();
			rookToSquare->setPiece(std::move(rookPiece));
			rookToSquare->getPiece()->addHasMoved();
		}
	}

	TemporaryMove(const TemporaryMove &) = delete;
	TemporaryMove &operator=(const TemporaryMove &) = delete;

	~TemporaryMove()
	{
		if (rookFromSquare != nullptr)
		{
			rookPiece = rookToSquare->releasePiece();
			rookPiece->subtractHasMoved();
			rookFromSquare->setPiece(std::move(rookPiece));
		}
		movingPiece = toSquare.releasePiece();
		movingPiece->subtractHasMoved();
		fromSquare.setPiece(std::move(movingPiece));
		captureSquare.setPiece(std::move(capturedPiece));
	}

private:
	Square &fromSquare;
	Square &toSquare;
	Square &captureSquare;
	std::unique_ptr<Piece> movingPiece;
	std::unique_ptr<Piece> capturedPiece;
	Square *rookFromSquare = nullptr;
	Square *rookToSquare = nullptr;
	std::unique_ptr<Piece> rookPiece;
};

bool isLegalCastlingShape(const Board &board, const Position &from,
						  const Position &to, const Piece &king)
{
	if (king.getType() != PieceType::KING || king.getHasMoved() != 0 ||
		from.getX() != to.getX() || from.getY() != 4 ||
		(from.getX() != (king.getColor() == Color::White ? 7 : 0)) ||
		(to.getY() != 2 && to.getY() != 6) ||
		!board[to.getX()][to.getY()].isEmpty())
		return false;

	const bool kingside = to.getY() == 6;
	const int rookColumn = kingside ? 7 : 0;
	const Piece *rook = board[from.getX()][rookColumn].getPiece();
	if (rook == nullptr || rook->getType() != PieceType::ROOK ||
		rook->getColor() != king.getColor() || rook->getHasMoved() != 0)
		return false;

	const Color opponent = king.getColor() == Color::White ? Color::Black : Color::White;
	const int firstEmptyColumn = kingside ? 5 : 1;
	const int lastEmptyColumn = kingside ? 5 : 3;
	for (int column = firstEmptyColumn; column <= lastEmptyColumn; ++column)
		if (!board[from.getX()][column].isEmpty())
			return false;

	return !ChessUtils::isSquareAttacked(board, from, opponent) &&
		   !ChessUtils::isSquareAttacked(board, Position(from.getX(), kingside ? 5 : 3), opponent) &&
		   !ChessUtils::isSquareAttacked(board, to, opponent);
}

bool isValidEnPassant(const Board &board, const Position &from,
					  const Position &to, const Piece &pawn,
					  const Move &lastMove)
{
	const int direction = pawn.getColor() == Color::White ? -1 : 1;
	const bool isDiagonalStep =
		to.getX() == from.getX() + direction &&
		std::abs(to.getY() - from.getY()) == 1;
	if (!isDiagonalStep || !board[to.getX()][to.getY()].isEmpty())
		return false;

	const Position adjacentPosition(from.getX(), to.getY());
	const Piece *adjacentPiece =
		board[adjacentPosition.getX()][adjacentPosition.getY()].getPiece();
	return adjacentPiece != nullptr &&
		   adjacentPiece->getType() == PieceType::PAWN &&
		   adjacentPiece->getColor() != pawn.getColor() &&
		   lastMove.getPieceType() == PieceType::PAWN &&
		   lastMove.getTo() == adjacentPosition &&
		   lastMove.getFrom().getX() == from.getX() + 2 * direction &&
		   lastMove.getTo().getX() == from.getX();
}

std::vector<CandidateMove> candidateMoves(Board &board, const Position &from,
										  Piece &piece, const Move *lastMove)
{
	std::vector<CandidateMove> candidates;
	for (const Position &to : piece.getValidMoves(from, board))
		candidates.push_back({to, false});

	if (piece.getType() == PieceType::KING)
	{
		for (const int column : {2, 6})
		{
			const Position to(from.getX(), column);
			if (isLegalCastlingShape(board, from, to, piece))
				candidates.push_back({to, false, true});
		}
	}

	const int enPassantRow = piece.getColor() == Color::White ? 3 : 4;
	if (piece.getType() != PieceType::PAWN || lastMove == nullptr ||
		from.getX() != enPassantRow)
		return candidates;

	for (const Position &to :
		 piece.getValidMoves(from, board, CustomMoveType::EN_PASSANT))
	{
		if (isValidEnPassant(board, from, to, piece, *lastMove))
			candidates.push_back({to, true});
	}
	return candidates;
}

bool isLegalCandidate(Board &board, const Position &from,
					  const CandidateMove &candidate, Color color)
{
	const Position &to = candidate.destination;
	if (board[to.getX()][to.getY()].getPiece() != nullptr &&
		board[to.getX()][to.getY()].getPiece()->getType() == PieceType::KING)
		return false;

	if (candidate.castling)
		if (!isLegalCastlingShape(board, from, to,
								  *board[from.getX()][from.getY()].getPiece()))
			return false;

	TemporaryMove simulation(board, from, to, candidate.enPassant,
							 candidate.castling);
	return !ChessUtils::isInCheck(board, color);
}
} // namespace

GameController::GameController(ChessScene *scene, QObject *parent)
	: QObject(parent), m_scene(scene)
{
	connect(m_scene, &ChessScene::squareClicked,
			this, &GameController::onSquareClicked);

	m_game.startGame(Color::White); // lays out the board; White moves first
	refreshView();
}

ChessScene::Board GameController::snapshot() const
{
	ChessScene::Board out{};
	const Board &board = m_game.getBoard();
	for (int row = 0; row < ChessScene::kBoardSize; ++row)
	{
		for (int column = 0; column < ChessScene::kBoardSize; ++column)
		{
			const Square &square = board[row][column];
			if (square.isEmpty())
				continue;
			const Piece *piece = square.getPiece();
			out[row][column] = PieceView{toKind(piece->getType()),
										 piece->getColor() == Color::White};
		}
	}
	return out;
}

std::vector<Position> GameController::legalMovesFrom(const Position &from)
{
	std::vector<Position> legal;

	Board &board = m_game.getBoard();
	Piece *piece = board[from.getX()][from.getY()].getPiece();
	if (piece == nullptr)
		return legal;

	const Color color = piece->getColor();
	for (const CandidateMove &candidate :
		 candidateMoves(board, from, *piece, m_game.getLastMove()))
		if (isLegalCandidate(board, from, candidate, color))
			legal.push_back(candidate.destination);
	return legal;
}

void GameController::onSquareClicked(int row, int column)
{
	if (row < 0 || row >= ChessScene::kBoardSize ||
		column < 0 || column >= ChessScene::kBoardSize)
		return;

	const Position clicked(row, column);

	if (m_selected)
	{
		try
		{
			m_game.makeMove(*m_selected, clicked);
			m_selected.reset();
			m_scene->clearHighlights();
			refreshView();
			announceGameEndIfAny();
			return;
		}
		catch (const InvalidMoveException &)
		{
			// Not a legal move for the selected piece -- fall through and
			// treat this click as a fresh selection attempt instead.
		}
	}

	trySelect(clicked);
}

void GameController::trySelect(const Position &clicked)
{
	m_scene->clearHighlights();
	m_selected.reset();

	const Board &board = m_game.getBoard();
	const Square &square = board[clicked.getX()][clicked.getY()];
	if (square.isEmpty() || square.getPiece()->getColor() != m_game.getCurrentTurn())
		return;

	const Piece *piece = square.getPiece();
	m_selected = clicked;

	QVector<QPoint> targets;
	QVector<QPoint> castleTargets;
	for (const Position &to : legalMovesFrom(clicked))
	{
		const QPoint target(to.getY(), to.getX()); // QPoint: x = column, y = row
		if (piece->getType() == PieceType::KING &&
			std::abs(to.getY() - clicked.getY()) == 2)
			castleTargets.push_back(target);
		else
			targets.push_back(target);
	}
	m_scene->highlightSquares(targets, castleTargets);
}

void GameController::refreshView()
{
	m_scene->showPosition(snapshot());
	emit statusChanged(statusText());
}

QString GameController::statusText() const
{
	const Color turn = m_game.getCurrentTurn();
	const QString side = (turn == Color::White) ? tr("White") : tr("Black");
	if (ChessUtils::isInCheck(m_game.getBoard(), turn))
		return tr("%1 to move \xE2\x80\x94 check!").arg(side); // em dash
	return tr("%1 to move").arg(side);
}

void GameController::announceGameEndIfAny()
{
	Board &board = m_game.getBoard();
	const Color turn = m_game.getCurrentTurn(); // side to move now, possibly mated/stalemated

	if (ChessUtils::isCheckmate(board, turn))
	{
		const QString winner = (turn == Color::White) ? tr("Black") : tr("White");
		emit statusChanged(tr("Checkmate \xE2\x80\x94 %1 wins!").arg(winner));
		return;
	}
	if (ChessUtils::isStalemate(board, turn))
	{
		emit statusChanged(tr("Stalemate \xE2\x80\x94 draw."));
		return;
	}
	if (ChessUtils::isFiftyMoveRuleDraw(m_game.getHalfMoveClock()))
	{
		emit statusChanged(tr("Draw by the 50-move rule."));
		return;
	}
	// No terminal condition; keep the normal "X to move" / check status.
}
