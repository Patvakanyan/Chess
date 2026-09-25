#include "chess/utils/chessUtils.hpp"
#include "chess/moveHistory.hpp"
#include "chess/game.hpp"

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

	bool protectedQueenCheckmatesKing()
	{
		Board board;
		board.initializeBoard();
		board.placePiece(new King(Color::Black), Position(0, 0));
		board.placePiece(new Queen(Color::White), Position(1, 1));
		board.placePiece(new King(Color::White), Position(2, 2));
		return ChessUtils::isInCheck(board, Color::Black) &&
			   !ChessUtils::hasLegalMoves(board, Color::Black) &&
			   ChessUtils::isCheckmate(board, Color::Black);
	}

	bool checkWithKingEscapeIsNotCheckmate()
	{
		Board board;
		board.initializeBoard();
		board.placePiece(new King(Color::Black), Position(0, 0));
		board.placePiece(new Queen(Color::White), Position(1, 1));
		board.placePiece(new King(Color::White), Position(3, 3));
		return ChessUtils::isInCheck(board, Color::Black) &&
			   !ChessUtils::isCheckmate(board, Color::Black);
	}

	bool stalemateIsNotCheckmate()
	{
		Board board;
		board.initializeBoard();
		board.placePiece(new King(Color::Black), Position(0, 0));
		board.placePiece(new Queen(Color::White), Position(1, 2));
		board.placePiece(new King(Color::White), Position(2, 2));
		return ChessUtils::isStalemate(board, Color::Black) &&
			   !ChessUtils::isCheckmate(board, Color::Black);
	}

	bool ordinaryPositionIsNeitherCheckmateNorStalemate()
	{
		Board board;
		board.initializeBoard();
		board.placePiece(new King(Color::White), Position(4, 4));
		board.placePiece(new King(Color::Black), Position(0, 0));
		return !ChessUtils::isInCheck(board, Color::White) &&
			   ChessUtils::hasLegalMoves(board, Color::White) &&
			   !ChessUtils::isCheckmate(board, Color::White) &&
			   !ChessUtils::isStalemate(board, Color::White);
	}

	bool fiftyMoveRuleUses100HalfMoves()
	{
		Board board;
		board.initializeBoard();
		board.placePiece(new King(Color::White), Position(4, 4));
		board.placePiece(new King(Color::Black), Position(0, 0));
		return !ChessUtils::isStalemate(board, Color::White) &&
			   !ChessUtils::isFiftyMoveRuleDraw(99) &&
			   ChessUtils::isFiftyMoveRuleDraw(100);
	}

	bool undoRestoresCapturedPieceMovementState()
	{
		Board board;
		board.placePiece(new King(Color::White), Position(7, 7));
		board.placePiece(new King(Color::Black), Position(7, 0));
		board.placePiece(new Rook(Color::White), Position(0, 0));
		board.placePiece(new Rook(Color::Black), Position(0, 2));

		MoveHistory history;
		Move whiteRookMove(Position(0, 0), Position(0, 1));
		whiteRookMove.makeMove(board);
		history.addMove(whiteRookMove);

		Move blackRookCapture(Position(0, 2), Position(0, 1));
		blackRookCapture.makeMove(board);
		history.addMove(blackRookCapture);

		bool captureResetsClock = history.getHalfMoveClock() == 0;
		return captureResetsClock && history.undoMove(board) &&
			   board[0][1].getPiece()->getHasMoved() == 1;
	}

	bool kingCannotBeCaptured()
	{
		Board board;
		board.placePiece(new King(Color::White), Position(7, 7));
		board.placePiece(new King(Color::Black), Position(0, 0));
		board.placePiece(new Queen(Color::White), Position(1, 1));

		try
		{
			Move captureKing(Position(1, 1), Position(0, 0));
			captureKing.makeMove(board);
		}
		catch (const InvalidMoveException &)
		{
			return true;
		}
		return false;
	}

	bool pawnDoubleStepRequiresStartingRank()
	{
		Board board;
		Pawn whitePawn(Color::White);
		Pawn blackPawn(Color::Black);

		return !whitePawn.isValidMove(Position(4, 0), Position(2, 0), board) &&
			   !blackPawn.isValidMove(Position(3, 1), Position(5, 1), board) &&
			   whitePawn.isValidMove(Position(6, 0), Position(4, 0), board) &&
			   blackPawn.isValidMove(Position(1, 1), Position(3, 1), board);
	}

	bool aNewGameClearsMoveHistory()
	{
		Game game;
		game.startGame(Color::White);
		game.makeMove(Position(7, 1), Position(5, 2));
		game.startGame(Color::Black);

		bool historyCleared = false;
		try
		{
			game.undoMove();
		}
		catch (const InvalidMoveException &)
		{
			historyCleared = true;
		}
		return historyCleared && game.getHalfMoveClock() == 0 && game.getCurrentTurn() == Color::Black;
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
	allPassed &= runTest("a protected queen checkmates the king",
						 protectedQueenCheckmatesKing());
	allPassed &= runTest("a king escape prevents checkmate",
						 checkWithKingEscapeIsNotCheckmate());
	allPassed &= runTest("stalemate is not checkmate", stalemateIsNotCheckmate());
	allPassed &= runTest("an ordinary position is neither checkmate nor stalemate",
						 ordinaryPositionIsNeitherCheckmateNorStalemate());
	allPassed &= runTest("the fifty-move rule is a draw after 100 half-moves",
						 fiftyMoveRuleUses100HalfMoves());
	allPassed &= runTest("undo restores a captured piece's movement state",
						 undoRestoresCapturedPieceMovementState());
	allPassed &= runTest("a king cannot be captured", kingCannotBeCaptured());
	allPassed &= runTest("pawn double moves require the starting rank",
						 pawnDoubleStepRequiresStartingRank());
	allPassed &= runTest("starting a new game clears move history",
						 aNewGameClearsMoveHistory());

	return allPassed ? 0 : 1;
}
