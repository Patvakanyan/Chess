#pragma once

#include "./square.hpp"
#include "./position.hpp"
#include <stdexcept>

#include "../include/bishop.hpp"
#include "../include/king.hpp"
#include "../include/knight.hpp"
#include "../include/pawn.hpp"
#include "../include/queen.hpp"
#include "../include/rook.hpp"
#include <iostream>


class Board
{
private:
	Square squares_[8][8];
	void printPieceTypes(const Position &position) const;
public:
	Board();
	Board(const Board &other) = delete;
	Board &operator=(const Board &other) = delete;
	Board(Board &&other) = default;
	Board &operator=(Board &&other) = default;
	~Board() = default;
	Square *operator[](size_t index);
	const Square *operator[](size_t index) const;

	void movePiece(const Position &from, const Position &to);
	void placePiece(Piece *piece, const Position &position);
	Piece *removePiece(const Position &position);
	bool isValidPosition(const Position &position) const;
	void initializeBoard();
};
