#pragma once

#include <QObject>
#include <QPoint>
#include <QString>
#include <QVector>

#include <optional>
#include <vector>

#include "PromotionDialog.hpp"
#include "chess/Game.hpp"
#include "chess/Position.hpp"
#include "ChessScene.hpp"
#include "GameMode.hpp"

class GameController : public QObject
{
	Q_OBJECT

public:
	explicit GameController(ChessScene *scene, QObject *parent = nullptr, GameMode mode = GameMode::VsStockfish);

signals:
	void statusChanged(const QString &text);

private slots:
	void onSquareClicked(int row, int column);

private:
	ChessScene::Board snapshot() const;

	std::vector<Position> legalMovesFrom(const Position &from);
	bool isClickAllowed(int row, int column);
	bool tryMoveSelected(const Position &clicked);
	void playEngineMoveIfNeeded();
	void trySelect(const Position &clicked);
	void refreshView();
	void announceGameEndIfAny();
	void clearSelection();
	bool isGameFinished();
	QString statusText() const;

	bool m_engineThinking = false;
	bool m_moveAnimating = false;
	bool m_engineAvailable = true;
	Game m_game;
	GameMode m_mode;
	ChessScene *m_scene;
	std::optional<Position> m_selected;
};
