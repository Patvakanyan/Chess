#pragma once

#include "position.hpp"
#include "piece.hpp"
#include "color.hpp"

class Square
{
private:
	Piece *piece_;
	Color color_;

public:
	Square(Piece *piece = nullptr, Color color = Color::Black);

	Square(const Square &other) = delete;
	Square &operator=(const Square &other) = default;
	~Square() = default;

	Piece *getPiece() const;
	void setPiece(Piece *piece);
	void setColor(Color color);

	bool isEmpty() const;
};
