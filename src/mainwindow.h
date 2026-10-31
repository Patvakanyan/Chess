#pragma once

#include <QMainWindow>

class ChessScene;
class ChessView;
class GameController;
class QFrame;
class QLabel;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

private:
	QFrame *createControlPanel(QWidget *parent);

	

	ChessScene *m_scene = nullptr;
	ChessView *m_boardView = nullptr;
	GameController *m_controller = nullptr;
	QLabel *m_statusLabel = nullptr;
};
