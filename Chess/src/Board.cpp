#include "chess/Board.hpp"

Board::Board() : squares_{}
{
}

void Board::placePiece(std::unique_ptr<Piece> piece, const Position &position)
{
	if (!isValidPosition(position))
		throw std::out_of_range("Piece position must be within the board.");

	Square *square = &squares_[position.getX()][position.getY()];
	square->setPiece(std::move(piece));
}

bool Board::isValidPosition(const Position &position) const
{
	int x = position.getX();
	int y = position.getY();
	return (x >= 0 && x < 8 && y >= 0 && y < 8);
}

Square *Board::operator[](size_t index)
{
	if (index >= 8)
		throw std::out_of_range("Row index must be between 0 and 7.");
	return squares_[index];
}

const Square *Board::operator[](size_t index) const
{
	if (index >= 8)
		throw std::out_of_range("Row index must be between 0 and 7.");
	return squares_[index];
}

std::unique_ptr<Piece> Board::removePiece(const Position &position)
{
	if (!isValidPosition(position))
		throw std::out_of_range("Piece position must be within the board.");

	Square *square = &squares_[position.getX()][position.getY()];
	return square->releasePiece();
}

void Board::initializeBoard()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			Color squareColor = ((i + j) % 2 == 0) ? Color::White : Color::Black;
			squares_[i][j].setPiece(nullptr);
			squares_[i][j].setColor(squareColor);
		}
	}
}
