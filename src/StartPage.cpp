#include "StartPage.hpp"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
	constexpr int kModeButtonWidth = 340;
	constexpr int kModeButtonHeight = 90;

	QString startPageStyleSheet()
	{
		return QStringLiteral(
			"QLabel#startTitle { color: #f7e6c4; font-size: 44px; font-weight: 800; }"
			"QLabel#startSubtitle { color: #aec0ad; font-size: 16px; }"
			"QPushButton#modeButton {"
			"  background-color: #293830; color: #f7e6c4;"
			"  border: 2px solid #536b5c; border-radius: 12px;"
			"  font-size: 16px; padding: 10px; }"
			"QPushButton#modeButton:hover { background-color: #35483d; border-color: #f1e0c2; }"
			"QPushButton#modeButton:pressed { background-color: #1f2924; }");
	}
} // namespace


StartPage::StartPage(QWidget *parent)
	: QWidget(parent)
{
	setStyleSheet(startPageStyleSheet());

	auto *title = new QLabel(tr("Chess"), this);
	title->setObjectName(QStringLiteral("startTitle"));
	title->setAlignment(Qt::AlignCenter);

	auto *subtitle = new QLabel(tr("Choose a game mode"), this);
	subtitle->setObjectName(QStringLiteral("startSubtitle"));
	subtitle->setAlignment(Qt::AlignCenter);

	QPushButton *twoPlayers = createModeButton(
		tr("2 Players"), tr("Play against a friend on this computer"));
	QPushButton *vsStockfish = createModeButton(
		tr("Play vs Stockfish"), tr("You are White, the computer plays Black"));

	connect(twoPlayers, &QPushButton::clicked, this,
			[this]() { emit modeSelected(GameMode::TwoPlayers); });
	connect(vsStockfish, &QPushButton::clicked, this,
			[this]() { emit modeSelected(GameMode::VsStockfish); });

	auto *layout = new QVBoxLayout(this);
	layout->setSpacing(16);
	layout->addStretch(2);
	layout->addWidget(title);
	layout->addWidget(subtitle);
	layout->addSpacing(24);
	layout->addWidget(twoPlayers, 0, Qt::AlignHCenter);
	layout->addWidget(vsStockfish, 0, Qt::AlignHCenter);
	layout->addStretch(3);
}

QPushButton *StartPage::createModeButton(const QString &title, const QString &description)
{
	// Two lines of text on one button: the mode name, then a short hint.
	auto *button = new QPushButton(title + QLatin1Char('\n') + description, this);
	button->setObjectName(QStringLiteral("modeButton"));
	button->setFixedSize(kModeButtonWidth, kModeButtonHeight);
	button->setCursor(Qt::PointingHandCursor);
	return button;
}
