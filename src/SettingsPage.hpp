#pragma once

#include <QWidget>

#include "chess/utils/Color.hpp"

class QCheckBox;
class QRadioButton;
class QSpinBox;

class SettingsPage : public QWidget
{
	Q_OBJECT

public:
	explicit SettingsPage(QWidget *parent = nullptr);
	void setSettings(int stockfishDepth, bool soundEnabled, Color playerColor = Color::White);

signals:
	void settingsSaved(int stockfishDepth, bool soundEnabled, Color playerColor);
	void backRequested();

private:
	QSpinBox *m_depthSpinBox = nullptr;
	QCheckBox *m_soundCheckBox = nullptr;
	QRadioButton *m_whiteColorButton = nullptr;
	QRadioButton *m_blackColorButton = nullptr;
};
