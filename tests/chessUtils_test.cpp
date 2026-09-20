#include "chess/chessUtils.hpp"

#include <iostream>
#include <string>

namespace
{
	const Position kingPosition(4, 4);

	bool isCheck(Color kingColor, Piece *attacker, const Position &attackerPosition)
	{
		Board board;
		board.initializeBoard();
		board.placePiece(new King(kingColor), kingPosition);
		board.placePiece(attacker, attackerPosition);
		return ChessUtils::isInCheck(board, kingColor);
	}

	bool blockedRookIsNotCheck()
	{
		Board board;
		board.initializeBoard();
		board.placePiece(new King(Color::White), kingPosition);
		board.placePiece(new Rook(Color::Black), Position(0, 4));
		board.placePiece(new Pawn(Color::White), Position(2, 4));
		return !ChessUtils::isInCheck(board, Color::White);
	}

	bool runTest(const std::string &name, bool result)
	{
		std::cout << (result ? "PASS: " : "FAIL: ") << name << '\n';
		return result;
	}
}

int main()
{
	bool allPassed = true;

	allPassed &= runTest("black rook checks white king",
						 isCheck(Color::White, new Rook(Color::Black), Position(4, 0)));
	allPassed &= runTest("white rook checks black king",
						 isCheck(Color::Black, new Rook(Color::White), Position(4, 0)));
	allPassed &= runTest("black bishop checks white king",
						 isCheck(Color::White, new Bishop(Color::Black), Position(1, 1)));
	allPassed &= runTest("white bishop checks black king",
						 isCheck(Color::Black, new Bishop(Color::White), Position(1, 1)));
	allPassed &= runTest("black queen checks white king",
						 isCheck(Color::White, new Queen(Color::Black), Position(4, 0)));
	allPassed &= runTest("white queen checks black king",
						 isCheck(Color::Black, new Queen(Color::White), Position(4, 0)));
	allPassed &= runTest("black knight checks white king",
						 isCheck(Color::White, new Knight(Color::Black), Position(2, 3)));
	allPassed &= runTest("white knight checks black king",
						 isCheck(Color::Black, new Knight(Color::White), Position(2, 3)));
	allPassed &= runTest("black pawn checks white king",
						 isCheck(Color::White, new Pawn(Color::Black), Position(5, 3)));
	allPassed &= runTest("white pawn checks black king",
						 isCheck(Color::Black, new Pawn(Color::White), Position(3, 3)));
	allPassed &= runTest("black king checks white king",
						 isCheck(Color::White, new King(Color::Black), Position(3, 3)));
	allPassed &= runTest("white king checks black king",
						 isCheck(Color::Black, new King(Color::White), Position(3, 3)));
	allPassed &= runTest("a piece blocks a rook check", blockedRookIsNotCheck());

	return allPassed ? 0 : 1;
}
