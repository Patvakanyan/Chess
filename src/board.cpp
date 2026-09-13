#include "../include/board.hpp"

Board::Board() : squares_{}
{
}

void Board::placePiece(Piece *piece, const Position &position)
{
	Square *square = &squares_[position.getX()][position.getY()];
	square->setPiece(piece);
}

bool Board::isValidPosition(const Position &position) const
{
	int x = position.getX();
	int y = position.getY();
	return (x >= 0 && x < 8 && y >= 0 && y < 8 && squares_[x][y].isEmpty());
}

Square *Board::operator[](size_t index)
{
	if (index >= 8)
	{
		throw std::out_of_range("Row index must be between 0 and 7.");
	}
	return squares_[index];
}

const Square *Board::operator[](size_t index) const
{
	if (index >= 8)
	{
		throw std::out_of_range("Row index must be between 0 and 7.");
	}
	return squares_[index];
}

Piece *Board::removePiece(const Position &position)
{
	Square *square = &squares_[position.getX()][position.getY()];
	Piece *removedPiece = square->getPiece();
	square->setPiece(nullptr);
	return removedPiece;
}

void Board::initializeBoard()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			Color squareColor = ((i + j) % 2 == 0) ? Color::White : Color::Black;
			squares_[i][j] = Square(nullptr, squareColor);
		}
	}
}

void Board::movePiece(const Position &from, const Position &to)
{
	if (!isValidPosition(to))
	{
		throw std::invalid_argument("Invalid destination position.");
	}

	Square *fromSquare = &squares_[from.getX()][from.getY()];
	Square *toSquare = &squares_[to.getX()][to.getY()];

	Piece *pieceToMove = fromSquare->getPiece();
	if (pieceToMove == nullptr)
	{
		throw std::invalid_argument("No piece at the source position.");
	}
	if (pieceToMove->isValidMove(from, to, *this))
	{
		throw std::invalid_argument("Invalid move for the selected piece.");
	}
	pieceToMove->setHasMoved(true);
	toSquare->setPiece(pieceToMove);
	fromSquare->setPiece(nullptr);
}
