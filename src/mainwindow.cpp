#include "mainwindow.h"

#include "ChessScene.hpp"
#include "ChessView.hpp"
#include "GameController.hpp"
#include "SettingsPage.hpp"
#include "StartPage.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
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
	connect(m_startPage, &StartPage::settingsRequested,
			this, &MainWindow::showSettings);

	QSettings settings;
	m_stockfishDepth = settings.value(QStringLiteral("stockfish/depth"), 10).toInt();
	m_stockfishDepth = qBound(1, m_stockfishDepth, 20);
	m_soundEnabled = settings.value(QStringLiteral("audio/moveSounds"), true).toBool();
	const QString playerColor = settings.value(
		QStringLiteral("game/playerColor"), QStringLiteral("white")).toString();
	m_playerColor = playerColor == QStringLiteral("black") ? Color::Black : Color::White;

	m_settingsPage = new SettingsPage(m_stack);
	m_stack->addWidget(m_settingsPage);
	connect(m_settingsPage, &SettingsPage::settingsSaved,
			this, &MainWindow::saveSettings);
	connect(m_settingsPage, &SettingsPage::backRequested,
			this, &MainWindow::showMenu);
}

void MainWindow::startGame(GameMode mode)
{
	m_gamePage = new QWidget;

	auto *layout = new QHBoxLayout(m_gamePage);
	layout->setContentsMargins(14, 14, 14, 14);
	layout->setSpacing(14);

	// Everything below is a child of m_gamePage, so deleting the page
	// cleans up the scene, view and controller (and stops Stockfish).
	const bool blackAtBottom = mode == GameMode::VsStockfish && m_playerColor == Color::Black;
	m_scene = new ChessScene(m_gamePage, blackAtBottom);
	m_boardView = new ChessView(m_scene, m_gamePage);
	m_boardView->setMinimumSize(kMinimumBoardViewSize, kMinimumBoardViewSize);

	layout->addWidget(m_boardView, 1);
	layout->addWidget(createControlPanel(m_gamePage, mode));

	m_controller = new GameController(m_scene, m_gamePage, mode,
		m_stockfishDepth, m_soundEnabled, m_playerColor);
	connect(m_controller, &GameController::statusChanged,
			m_statusLabel, &QLabel::setText);

	m_stack->addWidget(m_gamePage);
	m_stack->setCurrentWidget(m_gamePage);
}

void MainWindow::showSettings()
{
	m_settingsPage->setSettings(m_stockfishDepth, m_soundEnabled, m_playerColor);
	m_stack->setCurrentWidget(m_settingsPage);
}

void MainWindow::saveSettings(int stockfishDepth, bool soundEnabled, Color playerColor)
{
	m_stockfishDepth = qBound(1, stockfishDepth, 20);
	m_soundEnabled = soundEnabled;
	m_playerColor = playerColor;

	QSettings settings;
	settings.setValue(QStringLiteral("stockfish/depth"), m_stockfishDepth);
	settings.setValue(QStringLiteral("audio/moveSounds"), m_soundEnabled);
	settings.setValue(QStringLiteral("game/playerColor"),
		m_playerColor == Color::Black ? QStringLiteral("black") : QStringLiteral("white"));

	showMenu();
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

	const QString playerSide = m_playerColor == Color::White ? tr("White") : tr("Black");
	const QString modeText = mode == GameMode::VsStockfish
		? tr("Mode: you (%1) vs Stockfish").arg(playerSide)
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

	QLabel *depthLabel = nullptr;
	if (mode == GameMode::VsStockfish)
	{
		depthLabel = new QLabel(tr("Stockfish depth: %1").arg(m_stockfishDepth), panel);
		depthLabel->setObjectName(QStringLiteral("panelSubtitle"));
	}

	auto *menuButton = new QPushButton(tr("Back to menu"), panel);
	menuButton->setObjectName(QStringLiteral("menuButton"));
	menuButton->setCursor(Qt::PointingHandCursor);
	connect(menuButton, &QPushButton::clicked, this, &MainWindow::showMenu);

	panelLayout->addWidget(title);
	panelLayout->addWidget(modeLabel);
	panelLayout->addWidget(subtitle);
	if (depthLabel != nullptr)
		panelLayout->addWidget(depthLabel);
	panelLayout->addSpacing(12);
	panelLayout->addWidget(m_statusLabel);
	panelLayout->addStretch();
	panelLayout->addWidget(menuButton);

	return panel;
}
