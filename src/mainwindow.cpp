#include "mainwindow.h"

#include "ChessScene.hpp"
#include "ChessView.hpp"
#include "GameController.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
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
		"QLabel#panelStatus { background-color: #35483d; color: #f7e6c4; border-radius: 6px; padding: 9px; font-size: 13px; }");
}
} // namespace

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setMinimumSize(kMinimumWindowWidth, kMinimumWindowHeight);

	auto *centralWidget = new QWidget(this);
	centralWidget->setStyleSheet(panelStyleSheet());
	setCentralWidget(centralWidget);

	auto *layout = new QHBoxLayout(centralWidget);
	layout->setContentsMargins(14, 14, 14, 14);
	layout->setSpacing(14);

	m_scene = new ChessScene(this);

	m_boardView = new ChessView(m_scene, centralWidget);
	m_boardView->setMinimumSize(kMinimumBoardViewSize, kMinimumBoardViewSize);

	layout->addWidget(m_boardView, 1);
	layout->addWidget(createControlPanel(centralWidget));

	// GameController owns the Game instance, starts it, and draws the
	// starting position into m_scene as soon as it is constructed.
	m_controller = new GameController(m_scene, this);
	connect(m_controller, &GameController::statusChanged,
			m_statusLabel, &QLabel::setText);
}

QFrame *MainWindow::createControlPanel(QWidget *parent)
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

	auto *subtitle = new QLabel(tr("Click a piece, then a highlighted square"), panel);
	subtitle->setObjectName(QStringLiteral("panelSubtitle"));
	subtitle->setWordWrap(true);

	m_statusLabel = new QLabel(tr("White to move"), panel);
	m_statusLabel->setObjectName(QStringLiteral("panelStatus"));
	m_statusLabel->setWordWrap(true);

	panelLayout->addWidget(title);
	panelLayout->addWidget(subtitle);
	panelLayout->addSpacing(12);
	panelLayout->addWidget(m_statusLabel);
	panelLayout->addStretch();

	return panel;
}
