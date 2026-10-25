#pragma once

#include "chess/Piece.hpp"
#include "chess/utils/Color.hpp"
#include <memory>

class Square
{
private:
	std::unique_ptr<Piece> piece_;
	Color color_;

public:
	Square(std::unique_ptr<Piece> piece = nullptr, Color color = Color::Black);
	~Square() = default;

	Square(const Square &other) = delete;
	Square &operator=(const Square &other) = delete;

	Piece *getPiece() const;
	void setPiece(std::unique_ptr<Piece> piece);
	std::unique_ptr<Piece> releasePiece();
	void setColor(Color color);

	bool isEmpty() const;
};
