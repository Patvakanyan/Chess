#pragma once

#include <QObject>
#include <QPoint>
#include <QString>
#include <QVector>

#include <optional>
#include <vector>

#include "chess/Game.hpp"
#include "chess/Position.hpp"

#include "ChessScene.hpp"

// Bridges ChessScene (Qt, drawing, mouse clicks) and Game (the chess engine,
// no Qt dependency). This is the only class that knows about both.
class GameController : public QObject
{
	Q_OBJECT

public:
	explicit GameController(ChessScene *scene, QObject *parent = nullptr);

signals:
	// Human-readable status text: whose move it is, check, checkmate, etc.
	void statusChanged(const QString &text);

private slots:
	void onSquareClicked(int row, int column);

private:
	ChessScene::Board snapshot() const;
	// Legal destinations for the piece at `from` -- getValidMoves() is only
	// pseudo-legal (it doesn't know about check), so each candidate move is
	// simulated and rejected if it leaves the mover's own king in check,
	// exactly the way ChessUtils::hasLegalMoves does internally.
	std::vector<Position> legalMovesFrom(const Position &from);
	void trySelect(const Position &clicked);
	void refreshView();
	void announceGameEndIfAny();
	QString statusText() const;

	Game m_game;
	ChessScene *m_scene;
	std::optional<Position> m_selected;
};
