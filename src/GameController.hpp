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


	std::vector<Position> legalMovesFrom(const Position &from);
	void trySelect(const Position &clicked);
	void refreshView();
	void announceGameEndIfAny();
	void clearSelection();
	bool isGameFinished();
	QString statusText() const;

	Game m_game;
	ChessScene *m_scene;
	std::optional<Position> m_selected;
};
