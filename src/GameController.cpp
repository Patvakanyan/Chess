#include "GameController.hpp"

#include "chess/Board.hpp"
#include "chess/Piece.hpp"
#include "chess/Square.hpp"
#include "chess/exceptions/InvalidMoveException.hpp"
#include "chess/utils/ChessUtils.hpp"

#include <algorithm>
#include <QApplication>

namespace
{
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

} // namespace

GameController::GameController(ChessScene *scene, QObject *parent)
	: QObject(parent), m_scene(scene)
{
	connect(m_scene, &ChessScene::squareClicked,
			this, &GameController::onSquareClicked);

	m_game.startGame(); // lays out the board; White moves first
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
	return m_game.legalMovesFrom(from);
}

void GameController::onSquareClicked(int row, int column)
{
	if (row < 0 || row >= ChessScene::kBoardSize ||
		column < 0 || column >= ChessScene::kBoardSize)
		return;
	if (isGameFinished())
		return;

	const Position clicked(row, column);

	if (m_selected)
	{
		PieceType promotionType = PieceType::NONE;
		const std::vector<Position> legal = legalMovesFrom(*m_selected);
		const bool isLegal = std::find(legal.begin(), legal.end(), clicked) != legal.end();
		if (isLegal && m_game.isPromotionMove(*m_selected, clicked))
		{
			const bool white = (m_game.getCurrentTurn() == Color::White);
			PromotionDialog dlg(white, QApplication::activeWindow());
			if (dlg.exec() != QDialog::Accepted)
				return; // player closed the dialog: cancel the move
			promotionType = dlg.selectedPiece();
		}
		try
		{
			m_game.makeMove(*m_selected, clicked, promotionType);
			clearSelection();
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
	if (isGameFinished())
		return;

	const Board &board = m_game.getBoard();
	const Square &square = board[clicked.getX()][clicked.getY()];
	if (square.isEmpty() || square.getPiece()->getColor() != m_game.getCurrentTurn())
		return;

	const PieceType pieceType = square.getPiece()->getType();
	m_selected = clicked;

	QVector<QPoint> targets;
	QVector<QPoint> castleTargets;
	for (const Position &to : legalMovesFrom(clicked))
	{
		const QPoint target(to.getY(), to.getX()); // QPoint: x = column, y = row
		if (pieceType == PieceType::KING &&
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
	if (ChessAttackUtils::isInCheck(m_game.getBoard(), turn))
		return tr("%1 to move \xE2\x80\x94 check!").arg(side); // em dash
	return tr("%1 to move").arg(side);
}

void GameController::announceGameEndIfAny()
{
	switch (m_game.getResult())
	{
	case ChessGameResult::Checkmate:
	{
		const Color turn = m_game.getCurrentTurn();
		clearSelection();
		const QString winner = (turn == Color::White) ? tr("Black") : tr("White");
		emit statusChanged(tr("Checkmate \xE2\x80\x94 %1 wins!").arg(winner));
		return;
	}
	case ChessGameResult::Stalemate:
		clearSelection();
		emit statusChanged(tr("Stalemate \xE2\x80\x94 draw."));
		return;
	case ChessGameResult::Draw:
		clearSelection();
		emit statusChanged(tr("Draw by the 50-move rule."));
		return;
	default:
		return;
	}
}

void GameController::clearSelection()
{
	m_selected.reset();
	m_scene->clearHighlights();
}

bool GameController::isGameFinished()
{
	return m_game.isGameOver();
}
