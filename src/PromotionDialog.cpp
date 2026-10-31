#include "PromotionDialog.hpp"
#include "ChessScene.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

PromotionDialog::PromotionDialog(bool white, QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Choose a piece"));

	struct Choice
	{
		PieceType type;
		PieceKind kind;
	};
	const Choice choices[] = {
		{PieceType::QUEEN, PieceKind::Queen},
		{PieceType::ROOK, PieceKind::Rook},
		{PieceType::BISHOP, PieceKind::Bishop},
		{PieceType::KNIGHT, PieceKind::Knight}};

	auto *row = new QHBoxLayout;
	row->setSpacing(10);

	for (const Choice &c : choices)
	{
		auto *btn = new QPushButton;
		btn->setIcon(QIcon(ChessScene::pieceIcon(c.kind, white, 128)));
		btn->setIconSize(QSize(64, 64));
		btn->setFixedSize(80, 80);
		btn->setCursor(Qt::PointingHandCursor);
		btn->setStyleSheet(
			"QPushButton { background: #f1e0c2; border: 2px solid #6d4e3f; border-radius: 8px; }"
			"QPushButton:hover { background: #ffe9a8; }");

		connect(btn, &QPushButton::clicked, this, [this, c]()
				{
			m_selected = c.type;
			accept(); });
		row->addWidget(btn);
	}

	auto *main = new QVBoxLayout(this);
	main->addWidget(new QLabel(tr("Promote pawn to:")));
	main->addLayout(row);
}
