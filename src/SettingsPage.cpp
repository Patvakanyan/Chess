#include "SettingsPage.hpp"

#include <QCheckBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QVBoxLayout>

namespace
{
	QString settingsPageStyleSheet()
	{
		return QStringLiteral(
			"QLabel#settingsTitle { color: #f7e6c4; font-size: 36px; font-weight: 800; }"
			"QLabel#settingsHint { color: #aec0ad; font-size: 13px; }"
			"QLabel { color: #f7e6c4; font-size: 15px; }"
			"QSpinBox { background-color: #35483d; color: #f7e6c4; border: 1px solid #536b5c; "
			"  border-radius: 6px; padding: 7px; min-width: 90px; }"
			"QCheckBox, QRadioButton { color: #f7e6c4; font-size: 15px; spacing: 9px; }"
			"QPushButton { background-color: #35483d; color: #f7e6c4; border: 1px solid #536b5c; "
			"  border-radius: 7px; padding: 9px 18px; font-size: 14px; }"
			"QPushButton:hover { background-color: #43584b; }"
			"QPushButton#saveButton { background-color: #536b5c; font-weight: 700; }"
			"QPushButton#saveButton:hover { background-color: #647f6d; }");
	}
} // namespace

SettingsPage::SettingsPage(QWidget *parent)
	: QWidget(parent)
{
	setStyleSheet(settingsPageStyleSheet());

	auto *title = new QLabel(tr("Settings"), this);
	title->setObjectName(QStringLiteral("settingsTitle"));
	title->setAlignment(Qt::AlignCenter);

	m_depthSpinBox = new QSpinBox(this);
	m_depthSpinBox->setRange(1, 20);
	m_depthSpinBox->setValue(10);
	m_depthSpinBox->setToolTip(tr("A larger depth makes Stockfish stronger but slower."));

	auto *depthHint = new QLabel(
		tr("1 is fast and easy. Higher values are stronger and take more time."), this);
	depthHint->setObjectName(QStringLiteral("settingsHint"));
	depthHint->setWordWrap(true);

	m_soundCheckBox = new QCheckBox(tr("Play a sound after each move"), this);
	m_soundCheckBox->setChecked(true);

	m_whiteColorButton = new QRadioButton(tr("White"), this);
	m_blackColorButton = new QRadioButton(tr("Black"), this);
	m_whiteColorButton->setObjectName(QStringLiteral("whiteColorButton"));
	m_blackColorButton->setObjectName(QStringLiteral("blackColorButton"));
	m_whiteColorButton->setChecked(true);
	m_whiteColorButton->setToolTip(tr("Play the first move against Stockfish."));
	m_blackColorButton->setToolTip(tr("Stockfish plays the first move when you choose Black."));

	auto *colorLayout = new QHBoxLayout;
	colorLayout->addWidget(m_whiteColorButton);
	colorLayout->addWidget(m_blackColorButton);
	colorLayout->addStretch();

	auto *formLayout = new QFormLayout;
	formLayout->setHorizontalSpacing(24);
	formLayout->setVerticalSpacing(14);
	formLayout->addRow(tr("Your color (vs Stockfish):"), colorLayout);
	formLayout->addRow(tr("Stockfish depth:"), m_depthSpinBox);

	auto *backButton = new QPushButton(tr("Back"), this);
	auto *saveButton = new QPushButton(tr("Save settings"), this);
	saveButton->setObjectName(QStringLiteral("saveButton"));
	backButton->setCursor(Qt::PointingHandCursor);
	saveButton->setCursor(Qt::PointingHandCursor);

	connect(backButton, &QPushButton::clicked,
			this, &SettingsPage::backRequested);
	connect(saveButton, &QPushButton::clicked, this,
			[this]()
			{
				const Color playerColor = m_blackColorButton->isChecked() ? Color::Black : Color::White;
				emit settingsSaved(m_depthSpinBox->value(), m_soundCheckBox->isChecked(), playerColor);
			});

	auto *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(backButton);
	buttonLayout->addStretch();
	buttonLayout->addWidget(saveButton);

	auto *contentLayout = new QVBoxLayout;
	contentLayout->setSpacing(16);
	contentLayout->addLayout(formLayout);
	contentLayout->addWidget(depthHint);
	contentLayout->addSpacing(10);
	contentLayout->addWidget(m_soundCheckBox);
	contentLayout->addSpacing(18);
	contentLayout->addLayout(buttonLayout);

	auto *layout = new QVBoxLayout(this);
	layout->setContentsMargins(40, 30, 40, 30);
	layout->addStretch();
	layout->addWidget(title);
	layout->addSpacing(24);
	layout->addLayout(contentLayout);
	layout->addStretch();
}

void SettingsPage::setSettings(int stockfishDepth, bool soundEnabled, Color playerColor)
{
	m_depthSpinBox->setValue(stockfishDepth);
	m_soundCheckBox->setChecked(soundEnabled);
	QRadioButton *colorButton = playerColor == Color::Black ? m_blackColorButton : m_whiteColorButton;
	colorButton->setChecked(true);
}
