#pragma once

#include "chess/Position.hpp"
#include "chess/Piece.hpp"
#include "chess/utils/Color.hpp"

class Square
{
private:
	Piece *piece_;
	Color color_;

public:
	Square(Piece *piece = nullptr, Color color = Color::Black);
	~Square();

	Square(const Square &other) = delete;
	Square &operator=(const Square &other) = delete;

	Piece *getPiece() const;
	void setPiece(Piece *piece);
	Piece *releasePiece();
	void setColor(Color color);

	bool isEmpty() const;
};
