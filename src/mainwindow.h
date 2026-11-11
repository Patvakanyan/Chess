#pragma once

#include <QMainWindow>

#include "GameMode.hpp"

class ChessScene;
class ChessView;
class GameController;
class StartPage;
class QFrame;
class QLabel;
class QStackedWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

private slots:
	void startGame(GameMode mode); // menu -> game
	void showMenu();			   // game -> menu

private:
	QFrame *createControlPanel(QWidget *parent, GameMode mode);

	QStackedWidget *m_stack = nullptr;
	StartPage *m_startPage = nullptr;
	QWidget *m_gamePage = nullptr; // created fresh for every game

	ChessScene *m_scene = nullptr;
	ChessView *m_boardView = nullptr;
	GameController *m_controller = nullptr;
	QLabel *m_statusLabel = nullptr;
};
