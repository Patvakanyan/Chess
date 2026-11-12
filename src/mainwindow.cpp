#include "mainwindow.h"

#include "ChessScene.hpp"
#include "ChessView.hpp"
#include "GameController.hpp"
#include "StartPage.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
	constexpr int kMinimumWindowWidth = 800;
	constexpr int kMinimumWindowHeight = 600;
	constexpr int kMinimumBoardViewSize = 520;
	constexpr int kControlPanelWidth = 200;

	QString panelStyleSheet()
	{
		return QStringLiteral(
			"QWidget { background-color: #1f2924; color: #f6e7c9; }"
			"QLabel { background: transparent; }"
			"QFrame#controlPanel { background-color: #293830; border: 1px solid #536b5c; border-radius: 10px; }"
			"QLabel#panelTitle { color: #f7e6c4; font-size: 20px; font-weight: 700; }"
			"QLabel#panelSubtitle { color: #aec0ad; font-size: 12px; }"
			"QLabel#panelStatus { background-color: #35483d; color: #f7e6c4; border-radius: 6px; padding: 9px; font-size: 13px; }"
			"QPushButton#menuButton { background-color: #35483d; color: #f7e6c4; border: 1px solid #536b5c; border-radius: 6px; padding: 8px; }"
			"QPushButton#menuButton:hover { background-color: #43584b; }");
	}
} // namespace

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setMinimumSize(kMinimumWindowWidth, kMinimumWindowHeight);

	// The stack shows exactly one page at a time: the menu or the game.
	m_stack = new QStackedWidget(this);
	m_stack->setStyleSheet(panelStyleSheet());
	setCentralWidget(m_stack);

	m_startPage = new StartPage(m_stack);
	m_stack->addWidget(m_startPage);
	connect(m_startPage, &StartPage::modeSelected,
			this, &MainWindow::startGame);
}

void MainWindow::startGame(GameMode mode)
{
	m_gamePage = new QWidget;

	auto *layout = new QHBoxLayout(m_gamePage);
	layout->setContentsMargins(14, 14, 14, 14);
	layout->setSpacing(14);

	// Everything below is a child of m_gamePage, so deleting the page
	// cleans up the scene, view and controller (and stops Stockfish).
	m_scene = new ChessScene(m_gamePage);
	m_boardView = new ChessView(m_scene, m_gamePage);
	m_boardView->setMinimumSize(kMinimumBoardViewSize, kMinimumBoardViewSize);

	layout->addWidget(m_boardView, 1);
	layout->addWidget(createControlPanel(m_gamePage, mode));

	m_controller = new GameController(m_scene, m_gamePage, mode);
	connect(m_controller, &GameController::statusChanged,
			m_statusLabel, &QLabel::setText);

	m_stack->addWidget(m_gamePage);
	m_stack->setCurrentWidget(m_gamePage);
}

void MainWindow::showMenu()
{
	m_stack->setCurrentWidget(m_startPage);
	if (m_gamePage != nullptr)
	{
		m_stack->removeWidget(m_gamePage);
		m_gamePage->deleteLater(); // safe even though a button click triggered us
		m_gamePage = nullptr;
	}
	m_scene = nullptr;
	m_boardView = nullptr;
	m_controller = nullptr;
	m_statusLabel = nullptr;
}

QFrame *MainWindow::createControlPanel(QWidget *parent, GameMode mode)
{
	auto *panel = new QFrame(parent);
	panel->setObjectName(QStringLiteral("controlPanel"));
	panel->setFixedWidth(kControlPanelWidth);
	panel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

	auto *panelLayout = new QVBoxLayout(panel);
	panelLayout->setContentsMargins(18, 20, 18, 18);
	panelLayout->setSpacing(10);

	auto *title = new QLabel(tr("Chess board"), panel);
	title->setObjectName(QStringLiteral("panelTitle"));

	const QString modeText = (mode == GameMode::VsStockfish)
								 ? tr("Mode: you (White) vs Stockfish")
								 : tr("Mode: 2 players");
	auto *modeLabel = new QLabel(modeText, panel);
	modeLabel->setObjectName(QStringLiteral("panelSubtitle"));
	modeLabel->setWordWrap(true);

	auto *subtitle = new QLabel(tr("Click a piece, then a highlighted square"), panel);
	subtitle->setObjectName(QStringLiteral("panelSubtitle"));
	subtitle->setWordWrap(true);

	m_statusLabel = new QLabel(tr("White to move"), panel);
	m_statusLabel->setObjectName(QStringLiteral("panelStatus"));
	m_statusLabel->setWordWrap(true);

	auto *menuButton = new QPushButton(tr("Back to menu"), panel);
	menuButton->setObjectName(QStringLiteral("menuButton"));
	menuButton->setCursor(Qt::PointingHandCursor);
	connect(menuButton, &QPushButton::clicked, this, &MainWindow::showMenu);

	panelLayout->addWidget(title);
	panelLayout->addWidget(modeLabel);
	panelLayout->addWidget(subtitle);
	panelLayout->addSpacing(12);
	panelLayout->addWidget(m_statusLabel);
	panelLayout->addStretch();
	panelLayout->addWidget(menuButton);

	return panel;
}
