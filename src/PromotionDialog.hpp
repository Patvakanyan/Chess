#pragma once
#include <QDialog>
#include "chess/utils/EnumPiece.hpp"

class PromotionDialog : public QDialog
{
public:
	explicit PromotionDialog(bool white, QWidget *parent = nullptr);
	PieceType selectedPiece() const { return m_selected; }

private:
	PieceType m_selected = PieceType::QUEEN;
};
