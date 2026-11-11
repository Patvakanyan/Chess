#pragma once

#include <QWidget>

#include "GameMode.hpp"

class QPushButton;

// First screen of the app: the player picks a game mode.
class StartPage : public QWidget
{
	Q_OBJECT

public:
	explicit StartPage(QWidget *parent = nullptr);

signals:
	// Emitted when one of the mode buttons is clicked.
	void modeSelected(GameMode mode);

private:
	QPushButton *createModeButton(const QString &title, const QString &description);
};
