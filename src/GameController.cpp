#include "GameController.hpp"

#include "chess/Board.hpp"
#include "chess/Piece.hpp"
#include "chess/Square.hpp"
#include "chess/utils/ChessUtils.hpp"

#include <exception>
#include <memory>

namespace
{
// Engine rows/columns already match the GUI's: row 0 is rank 8 (top, black
// side), column 0 is the a-file (left), same as ChessScene. No flipping.
PieceKind toKind(PieceType type)
{
	switch (type)
	{
	case PieceType::KING:   return PieceKind::King;
	case PieceType::QUEEN:  return PieceKind::Queen;
	case PieceType::ROOK:   return PieceKind::Rook;
	case PieceType::BISHOP: return PieceKind::Bishop;
	case PieceType::KNIGHT: return PieceKind::Knight;
	case PieceType::PAWN:
	default:                return PieceKind::Pawn;
	}
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
	for (int row = 0; row < 8; ++row)
	{
		for (int column = 0; column < 8; ++column)
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

	struct CandidateMove
	{
		Position to;
		bool enPassant;
	};

	std::vector<CandidateMove> candidates;
	for (const Position &to : piece->getValidMoves(from, board))
		candidates.push_back({to, false});

	const Move *lastMove = m_game.getLastMove();
	const int direction = color == Color::White ? -1 : 1;
	const int enPassantRow = color == Color::White ? 3 : 4;
	if (piece->getType() == PieceType::PAWN &&
		lastMove != nullptr &&
		from.getX() == enPassantRow)
	{
		for (const Position &to : piece->getValidMoves(from, board, CustomMoveType::EN_PASSANT))
		{
			const bool isDiagonalStep =
				to.getX() == from.getX() + direction &&
				std::abs(to.getY() - from.getY()) == 1;
			const bool destinationIsEmpty = board[to.getX()][to.getY()].isEmpty();
			const Position adjacentPosition(from.getX(), to.getY());
			const Piece *adjacentPiece =
				board[adjacentPosition.getX()][adjacentPosition.getY()].getPiece();
			const bool capturesLastPawn =
				lastMove->getPieceType() == PieceType::PAWN &&
				lastMove->getTo() == adjacentPosition &&
				lastMove->getFrom().getX() == from.getX() + 2 * direction &&
				lastMove->getTo().getX() == from.getX();

			if (isDiagonalStep && destinationIsEmpty && adjacentPiece != nullptr &&
				adjacentPiece->getType() == PieceType::PAWN &&
				adjacentPiece->getColor() != color && capturesLastPawn)
				candidates.push_back({to, true});
		}
	}

	for (const CandidateMove &candidate : candidates)
	{
		const Position &to = candidate.to;
		Square &fromSquare = board[from.getX()][from.getY()];
		Square &toSquare = board[to.getX()][to.getY()];

		if (toSquare.getPiece() != nullptr &&
			toSquare.getPiece()->getType() == PieceType::KING)
			continue; // shouldn't happen, but never treat "capture the king" as legal

		// En passant captures the pawn beside the destination, not the
		// destination square itself.
		Square &captureSquare = candidate.enPassant
			? board[from.getX()][to.getY()]
			: toSquare;
		std::unique_ptr<Piece> capturedPiece = captureSquare.releasePiece();
		std::unique_ptr<Piece> movingPiece = fromSquare.releasePiece();
		toSquare.setPiece(std::move(movingPiece));
		piece->addHasMoved();

		const bool safe = !ChessUtils::isInCheck(board, color);

		movingPiece = toSquare.releasePiece();
		fromSquare.setPiece(std::move(movingPiece));
		captureSquare.setPiece(std::move(capturedPiece));
		piece->subtractHasMoved();

		if (safe)
			legal.push_back(to);
	}
	return legal;
}

void GameController::onSquareClicked(int row, int column)
{
	if (row < 0 || row >= 8 || column < 0 || column >= 8)
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
		catch (const std::exception &)
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

	m_selected = clicked;

	QVector<QPoint> targets;
	for (const Position &to : legalMovesFrom(clicked))
		targets.push_back(QPoint(to.getY(), to.getX())); // QPoint: x = column, y = row
	m_scene->highlightSquares(targets);
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
