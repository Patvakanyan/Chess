#pragma once

#include "chess/position.hpp"
#include "chess/piece.hpp"
#include "chess/utils/color.hpp"

class Square
{
private:
	Piece *piece_;
	Color color_;

public:
	Square(Piece *piece = nullptr, Color color = Color::Black);
	~Square();

	Square(const Square &other) = delete;
	Square &operator=(const Square &other) = default;

	Piece *getPiece() const;
	void setPiece(Piece *piece);
	void setColor(Color color);

	bool isEmpty() const;
};
