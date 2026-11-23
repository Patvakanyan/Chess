#pragma once

#include <QMainWindow>

#include "GameMode.hpp"
#include "chess/utils/Color.hpp"

class ChessScene;
class ChessView;
class GameController;
class StartPage;
class SettingsPage;
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
	void showMenu(); // game -> menu
	void showSettings();
	void saveSettings(int stockfishDepth, bool soundEnabled, Color playerColor);

private:
	QFrame *createControlPanel(QWidget *parent, GameMode mode);

	QStackedWidget *m_stack = nullptr;
	StartPage *m_startPage = nullptr;
	SettingsPage *m_settingsPage = nullptr;
	QWidget *m_gamePage = nullptr; // created fresh for every game

	int m_stockfishDepth = 10;
	bool m_soundEnabled = true;
	Color m_playerColor = Color::White;

	ChessScene *m_scene = nullptr;
	ChessView *m_boardView = nullptr;
	GameController *m_controller = nullptr;
	QLabel *m_statusLabel = nullptr;
};
